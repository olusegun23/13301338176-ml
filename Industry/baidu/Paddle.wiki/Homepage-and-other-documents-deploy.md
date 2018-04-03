# 子网站与实际目录对应
- www.paddlepaddle.org: 
  - site/home2
  - https://github.com/PaddlePaddle/portal.git
- doc.paddlepaddle.org: 
  - site/exp
  - https://github.com/PaddlePaddle/paddlepaddle.git
  - gh-pages
- models.paddlepaddle.org: 
  - site/models
  - https://github.com/Superjom/paddle.models.git
- blog.paddlepaddle.org:
  - 博客测试
  - site/blog
  - https://github.com/PaddlePaddle/portal.git, blog branch

所有的git需要迁回到 paddlepaddle ，后续统一整理

# 上线流程
- 目前开放 exp.paddlepaddle.org 域名作为测试用
- 先用 exp 测试服务，没有问题后修改 proxy 将服务迁回到正式域名

# blog 自动部署工作流
记录一下blog在服务器上部署的方式，其他子站可以借鉴

- 网站基于 jekyll
- 内容用 markdown 书写，方便 PR
- 服务器端使用 Canddy.git ，自动拉取 github 中对应仓库的markdown文件的修改
- 另外建立一个 `jekyll build -w` 的服务，当 markdown 文件更新，自动重新编译 html

# models 部署

- 脚本，实现从 paddle/models 中 markdown 到 jekyll 目录结构的修改
