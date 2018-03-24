My Main Page usig markdown                         {#mainpage}
============

http://www.stack.nl/~dimitri/doxygen/manual/markdown.html



https://segmentfault.com/markdown

1. 列出所有元素：
    - 无序列表元素 A
        1. 元素 A 的有序子列表
    - 前面加四个空格
2. 列表里的多段换行：
    前面必须加四个空格，
    这样换行，整体的格式不会乱
3. 列表里引用：

    > 前面空一行
    > 仍然需要在 >  前面加四个空格

4. 列表里代码段：

    ```
    前面四个空格，之后按代码语法 ``` 书写
    ```

        或者直接空八个，引入代码块



公式
当你需要在编辑器中插入数学公式时，可以使用两个美元符 $$ 包裹 TeX 或 LaTeX 格式的数学公式来实现。提交后，问答和文章页会根据需要加载 Mathjax 对数学公式进行渲染。如：


$$ x = {-b \pm \sqrt{b^2-4ac} \over 2a}. $$

$$
x \href{why-equal.html}{=} y^2 + 1
$$


同时也支持 HTML 属性，如：






https://www.jianshu.com/p/888c5eaebabd

方法三：使用MathJax引擎
大家都看过Stackoverflow上的公式吧，漂亮，其生成的不是图片。这就要用到MathJax引擎，在Markdown中添加MathJax引擎也很简单，

$$x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}$$\\(x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}\\)

然后，再使用Tex写公式。$$公式$$
表示行间公式，本来Tex中使用(公式)
表示行内公式，但因为Markdown中
是转义字符，所以在Markdown中输入行内公式使用\(公式\)
，如下代码：

$$x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}$$\\(x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}\\)


dd





First Header  | Second Header
------------- | -------------
Content Cell  | Content Cell 
Content Cell  | Content Cell 

block-level HTML elements — e.g. <div>, <table>, <pre>, <p>, etc. — must be separated from surrounding content by blank lines, and the start and end tags of the block should not be indented with tabs or spaces.


Here is text for one paragraph.

We continue with more text in another paragraph.

This is a level 1 header
========================

This is a level 2 header
------------------------

# This is a level 1 header

### This is level 3 header #######

> This is a block quote
> spanning multiple lines


0  if OK\n
>1 if NOK

- Item 1

  More text for this item.

- Item 2
  + nested list item.
  + another nested item.
- Item 3

[The link text](http://example.net/)
[The link text](http://example.net/ "Link title")
[The link text](/relative/path/to/index.html "Link title") 
[The link text](somefile.html) 


| Right | Center | Left  |
| ----: | :----: | :---- |
| 10    | 10     | 10    |
| 1000  | 1000   | 1000  |



| Right | Center | Left  |
| ----: | :----: | :---- |
| 10    | 10     | 10    |
| 1000  |||

Header 1                {#labelid}
========

## Header 2 ##          {#labelid2}

text

 text

  text

   code



