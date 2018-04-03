## TLDR
1. [I need to restart my build](#paddlepaddle-ci-dashboard)
1. [I need to check my build log](#view-build-log)
1. [I need to setup the whole CI system](#launch-teamcity-ci-system)
1. [I need to add an agent](#setup-teamcity-agents)
1. [I need to backup/restore teamcity](#i-need-to-backuprestore-teamcity)
1. [I need to upgrade teamcity](#i-need-to-upgrade-teamcity)
1. [General trouble shooting](#trouble-shooting)
1. [Add build agent license](#add-build-agent-license)

## PaddlePaddle's CI system

Continuous Integration (CI) is one of the formative concepts, driven by a need to regularly integrate new and changed code back into the master repository. Teams compile software, and run it through a series of tests in a production-identical development environment to ensure the success of the build.

[TeamCity](https://www.jetbrains.com/teamcity/?fromMenu) is a proprietary offering by [JetBrains](https://www.jetbrains.com/), which serve as CI tools that allow developers to integrate code branches during the development process and run a series of automated tests against them.

To reduce the burden of deploying and maintaining PaddlePaddle, we adopted a novel approach whereby we specifically built the latest PaddlePaddle image within TeamCity agent container, then run unit test based upon generated image.

### PaddlePaddle CI DashBoard

To login and monitor the status of tasks, simply click the following dashboard.

Current [CI DashBoard](https://paddleci.ngrok.io/)

You can login as a guest by clicking "Log in as guest".
For administrator login info, please ask in Paddle Hi Group.

#### View Build Log

1. Click on "Details" in the TeamCity build section.
[[images/ci_view_log_0.png]]

1. Login or click "Log in as guest".
1. You will be landed on the "Overview" tag, there are some truncated build log, that is **not** what we want:
[[images/ci_view_log_1.png]]
1. Click "Build Log". You can download the full log by clicking "Download full build log".
[[images/ci_view_log_2.png]]


#### Rebuild Pull Request

1. Visit https://paddleci.ngrok.io/ and login as admin (please ask from your colleague or in the Hi Group for the password)
1. Find the "PR_CI" section and click the "..." on right end (top right in the screenshot).
[[images/ci_rebuild_0.png]]
1. Select your PR branch (same as your PR number, "2133" in this example) and click "Run Build".
[[images/ci_rebuild_1.png]]

## Launch TeamCity CI System

Teamcity is a distributed builds system, which has 2 major roles in it, the master and agents. We are going to guide you through the process of setting up both parties.

### Launch teamcity master

We are going to launch teamcity master with [Docker Image distribution](https://hub.docker.com/r/jetbrains/teamcity-server/).

TeamCity stores build history, users, build results and some run time data in an SQL database. It comes with a pre-configured built-in HSQLDB. 
For the concerns of reliability and performance, we are going to setup an external database. In our case, we are going to use PostgreSQL.

#### Setup PostgreSQL Server

We are going to setup the PostgreSQL Server with docker.

```bash
docker run -d --name teamcity-db \
-p 5432:5432 \
-e POSTGRES_PASSWORD=XXXXXX \
postgres \
-c "synchronous_commit=off" -c 'shared_buffers=512MB' -c 'max_wal_size=1500MB' -c 'checkpoint_completion_target=0.9'
```

Command above starts a container with `postgres` images, named `teamcity-db`, exposed container port `5432` to host's port `5432`, postgres' db master password `XXXXXX`, and other db configurations recommended by teamcity with parameters followed by `-c`. The command will return the id of the container, we assume it as `09d34686` in this guide.

Now the db server is up, we need to create a database to hold the teamcity data. Let's connect to the container and run some SQL.

``` bash
docker exec -it 09d34686 bash
```

Now we are connected to the container. Now we need to run PostgreSQL client `psql` as user `postgres`

```bash
su - postgres
psql
```
We are in the `psql`, now let's create teamcity's database, and exit.

```bash
CREATE DATABASE teamcity \g
\q
```

PostgreSQL Server is now ready for teamcity.

#### Setup teamcity master

Let's start teamcity docker instance by running the following:

```bash
docker run -d --name teamcity-server-instance -v /home/teamcity_server/_data/:/data/teamcity_server/datadir -v /home/teamcity_server/logs/:/opt/teamcity/logs -p 8111:8111 jetbrains/teamcity-server
```

Above command started `jetbrains/teamcity-server` instance, and mounted data and log directories and exposed container port `8111` to host's `8111`. It will return the docker id of the teamcity-instance, let's assume it to be `9d41a5cb7c25` in this guide.

Now let's open a browser and open `http://localhost:8111` to finish basic setups.

When the UI prompt about database type, select `PostgreSQL`, then do as described in the UI to download `JDBC driver` and fill the rest of the form with following database credentials `Database Host: <Machine IP>, Database Name: teamcity, User name: postgres, Password: XXXXXX`, then click on the `Proceed` button.

Now your teamcity master is ready to go.

Note, you will need to login as admin to change some settings. In login window, there is an option to login as admin with token. After you click that link, run `docker logs 9d41a5cb7c25` in master machine, you will see the admin token at the last line of the log.

#### Routine Data backup

Teamcity has a script for database maintenance. We are going to utilize it to do daily data backup. Create a script as below and put it in teamcity container's `opt/teamcity/bin/daily_backup.sh`

```bash
rm -f /data/teamcity_server/datadir/backup/daily_backup.zi*
/opt/teamcity/bin/maintainDB.sh backup --all -F /data/teamcity_server/datadir/backup/daily_backup
```

Then add the following command in master server's crontab

``` bash
1 0 * * * docker exec -d 9d41a5cb7c25 bash /opt/teamcity/bin/daily_backup.sh
```

above command runs backup script everyday at 1am and store backup file at `/home/teamcity_server/_data/backup/daily_backup.zip`


#### Launch ngrok

The TeamCity server is behind a firewall, we are using ngrok as a tunnel for external access.

Run command: `~/ngrok http -subdomain paddleci 8111` to start ngrok.


#### Setup teamcity agents

As a distributed builds system, we still need to setup at least one agent to start building.

In our case, we recommend either Ubuntu or CentOS as host system. If you need to build IOS or OSX distributions, you will need to setup Mac Agents.

##### Before you run the agent

To properly build PaddlePaddle in Ubuntu or CentOS, makes sure you have the `nvidia-docker` properly installed.

Run the following command:
```bash
$ nvidia-docker run --rm nvidia/cuda:8.0-cudnn5-runtime-ubuntu16.04 nvidia-smi
```

If you see output similar to:
```text
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 375.51                 Driver Version: 375.51                    |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|===============================+======================+======================|
|   0  GeForce GTX TIT...  Off  | 0000:08:00.0     Off |                  N/A |
| 22%   41C    P8    27W / 250W |      0MiB / 12204MiB |      0%      Default |
+-------------------------------+----------------------+----------------------+

+-----------------------------------------------------------------------------+
| Processes:                                                       GPU Memory |
|  GPU       PID  Type  Process name                               Usage      |
|=============================================================================|
|  No running processes found                                                 |
+-----------------------------------------------------------------------------+
```

then nvidia-docker is working correctly, we can proceed to the next step.

If the command prints error message or got stuck, use `systemctl` to start `docker` and `nvidia-docker` services:

```bash
$ systemctl start docker
$ systemctl start nvidia-docker
```

If anything goes wrong starting the services, `systemctl status docker`, `systemctl status nvidia-docker`, `journalctl -xe` is your debugging friend.


##### Start the agent

We are going to push the agent binary from the master to agent machine and change some settings a bit in our case.

1. Push agent binary to agent machine

    Open `http://<master ip>:8111/agents.html?tab=agent.push`, then click on `Install agent ...` button. Then fill the `host`, `username`, `password` of the agent machine to have agent pushed.

1. Change `Server URL`

    SSH to your agent machine, open `$HOME/BuildAgent`, then edit `conf/buildAgent.properties`, change the `serverUrl` value from `https://paddleci.ngrok.io` to master's internal IP address.

    the reason to do so is to avoid dns name solving overhead.

1. Restart Agent

    In agent machine, run:
    ```bash
    sudo $HOME/BuildAgent/bin/agent.sh stop
    sudo $HOME/BuildAgent/bin/agent.sh start
    ```

1. Authorize the Agent

    Now you should be able to see the newly setup agent in `http://<master ip>:8111/agents.html?tab=unauthorizedAgents`

    Click `Authorize` button to add it to agent pool.

1. Routine docker cleanup

    add following command to crontab in agent machine

    ```bash
    0 0 * * * docker system prune -f -a
    ```

    above command will remove all unused container, image and volumes at 0:00 everyday

### Projects in CI

Currently, we have three projects on TeamCity CI system.

1. PaddlePaddle CPU and GPU Docker Images

    If new pull requests are merged into develop branch of PaddlePaddle, TeamCity will detect and build new PaddlePaddle images for both CPU and GPU. Then, they will be pushed into Docker Hub automatically.

2. PaddlePaddle GPU Unit Test

    If new pull requests are merged into develop branch of PaddlePaddle, TeamCity will pull the latest PaddlePaddle GPU image and run unit test to validate the modification. Most of pull requests are
    irrelevant to GPU, thus GPU Unit Test is triggered per hour by TeamCity.

3. PaddlePaddle Book CPU and GPU Docker Images

    If new pull requests are merged into develop branch of PaddlePaddle Book, TeamCity will detect and build new PaddlePaddle Book images for both CPU and GPU. Then, they will be pushed into Docker Hub automatically.

## Trouble Shooting

### I need to backup/restore teamcity

To back up the data, there are 2 ways to do so

1. Login to master machine, and run ` docker exec -d <teamcity container id> bash /opt/teamcity/bin/daily_backup.sh`, backup file will be located at `/home/teamcity_server/_data/backup/daily_backup.zip`

1. open `http://<master ip>:8111/admin/admin.html?item=backup` then `start backup`

To Restore the data, there are also 2 ways to do so

1. Login to master machine, and run ` docker exec -d <teamcity container id> bash /opt/teamcity/bin/maintainDB.sh restore -F <backup file path>`

1. open `http://<master ip>:8111/admin/admin.html?item=import`

### I need to upgrade teamcity

To upgrade teamcity is pretty simple, just pull the new image and start the server as follows

```bash
docker pull jetbrains/teamcity-server
docker run -d --name teamcity-server-instance -v /home/teamcity_server/_data/:/data/teamcity_server/datadir -v /home/teamcity_server/logs/:/opt/teamcity/logs -p 8111:8111 jetbrains/teamcity-server
```

Be sure to update crontab's daily data backup script with latest container id.

```bash
crontab -e
```

And update `1 0 * * * docker exec -d 9d41a5cb7c25 bash /opt/teamcity/bin/daily_backup.sh` container id.


### Run out of space in /data/teamcity_server/datadir

Go to http://172.19.32.197:8111/admin/admin.html?item=cleanup , click the button "Start clean-up now"

### How to Add New Projects in CI system

Please check out [TeamCity Documentation](https://confluence.jetbrains.com/display/TCD10/Creating+and+Editing+Projects) to find more details.


### Agent fails to update source

This is usually caused by running agent under non-root user. The solution is to login to agent machine and start agent with sudo

```bash
sudo <teamcity root>/bin/agent.sh start
```

### Agent runs out of space

This is usually caused by docker images taking too much space. try `docker system prune -f -a` to remove unused resources in docker to free some space.

If you agent is running in CentOS, there is another possibility: docker's overlay fs is running inside cl-root, which is limited to 50G in most of the case. The solution is to point docker graph path to a larger disk partition. To do so, I found [this](https://linuxconfig.org/how-to-move-docker-s-default-var-lib-docker-to-another-directory-on-ubuntu-debian-linux) and [this](https://forums.docker.com/t/how-do-i-change-the-docker-image-installation-directory/1169/21) very useful.

## Add Build Agent License

TeamCity offers 3 free build agents, additional agent needs to be purchased. You will be given a license key after the purchase. To register the license, login into the TeamCity webpage, click "Administration" on the top right corner, then click "Licenses".

## Reference

1. TeamCity vs Jenkins for Continuous Integration, https://www.upguard.com/articles/teamcity-vs.-jenkins-for-continuous-integration

