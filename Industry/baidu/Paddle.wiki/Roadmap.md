## Open Source

Codebase refactorizing by August 31; others by November 30.

1. PaddlePaddle
   1. Rewrite to make it easy to read and to contribute. (**codebase refactorizing**)
   1. Improve usability (**codebase refactorizing**)
       1. detailed error messages and hints.
       1. program distributed jobs in Web pages using Jupyter Notebook.
   1. Improve performance.
      1. Support float16 to make full use of Volta in training
      1. Fine tune the use of cuDNN for efficient training
      1. Integrate with TensorRT for serving

1. State Key Lab Cluster
    1. Servers from the State Key Lab of Deep Learning
    1. Use highly efficient multiple-user resource allocation system (Kubernetes)
    1. The web page for user account registration and log-in.
    1. Access distributed filesystem using a command-line tool.
    1. Launch and monitor distributed jobs using a command-line tool.
    1. **Data security challenge**: how could we prevent outbound link and leaking from data on the cluster?

1. PaddlePaddle Cloud
   1. Use same tech stack as the State Key Lab cluster.
   1. Deployable to enterprise clients.

## Open Data

1. Security data
   - According to 马杰, this would shock the network security society.
2. Video segmentation data
   - Current the largest dataset contains 80,000 images,
   - We can open one million images by July 5. Up to 10 million in the future.
3. Fine-grained image classification data
   - with Chinese labels
4. Medical image data
5. Speech data
6. NLP data

Owners of 5. and 6. might not want to open the data.

## Open Models

From now on to December 30

   1. Speech
      DeepSpeech DeepVoice, and DeepSpeaker
   1. Computer Vision
      - well-know open models in PaddlePaddle format: VGG, ResNet, GoogleNet 
      - classification, detection, and segmentation algorithms in classical papers
      - metric learning for recognition (publish later, internal use first)
   1. NLP
      - machine translation
      - text relevance comparison

## Open Course

From now on to December 30

- Describe above works in new chapters of book.paddlepaddle.org, and bring it into universities.
- Enrich online video courses (already recorded and to be published on 百度学院)
- Encourage students to use PaddlePaddle on PaddlePaddle Cloud.
- Intern program: push the limit of Deep Learning.