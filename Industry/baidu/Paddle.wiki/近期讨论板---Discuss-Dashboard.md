
### API documentation. Julie

### API implementation cleanup.
@reyoung
- [ ] network configuration – auto-gen Python code from C++ code.
- [ ] move network fulfillment and parameter creation from Python to C++.
- [ ] multi-language interface – remove SWIG, use C-API.
- [ ] ultimately, we can remove old API code.
- [ ] open source C interface of C++ inference library.

### refactorization
@hedaoyuan
- [ ] ”functions”
- [ ] overview, Xu Wei (TF Fold, PyTorch, DyNet)
- [ ] Go re-implementation @helinwang

### Kubernetes cluster
@typhoonzero
- [ ] CentOS or CoreOS
- [ ] auto upgrade Kuberentes
- [ ] user quota (computational and storage)
- [ ] kubectl and corresponding Python API (to run all kinds of jobs)

### fault recoverable PaddlePaddle
@helinwang
- [ ] checkbox dist_train
- [ ] reader – related with master and task dispatch.
- [ ] paddlectl and corresponding Python API

### Releasing and deployment
@gangliao
- [ ] runtime check AVX in Paddle for docker image – liaogang
- [ ] host China-local Dockerhub server

### PaddlePaddle Cloud
- [ ] Web site : cloud.paddlepaddle.org – user register/login, token,
- [ ] paddlectl API : paddle.{dist_}train(…, k8s_token = )

### application-specific tasks – image
@qingqing01
- [ ] need more operations/layers support segmentation and detection
- [ ] integrate Caffe features.

### new applications:
- [ ] deep speech 2
- [ ] deep voice

### book - more chapters. 
@lcy-seso

### ARM-based deployment.
@xreki