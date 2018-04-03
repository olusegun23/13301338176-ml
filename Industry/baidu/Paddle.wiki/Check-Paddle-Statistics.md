1. Check the total number of downloads of our .deb release packages:

   ```
curl -s -u wangkuiyi https://api.github.com/repos/PaddlePaddle/Paddle/releases  | grep download_count | cut -f 2 -d ':' | sed 's/ //g' | sed 's/,//g' | awk '{s+=$1;} END{print s;}'
   ```

1. Check the total pulls of Docker images:

   ```
curl  https://hub.docker.com/v2/repositories/paddlepaddle/paddle/  | grep pull_count  | sed "s/,/\n/g" | grep pull_count |awk -F: '{print $2}'
   ```

1. Check the number of git clones in recent 14 days:

   ```
curl -u wangkuiyi --header 'Accept: application/vnd.github.spiderman-preview' https://api.github.com/repos/PaddlePaddle/Paddle/traffic/clones
   ```

   For more details, please refer to Github Traffic API (in preview stage) [document](https://developer.github.com/v3/repos/traffic/).