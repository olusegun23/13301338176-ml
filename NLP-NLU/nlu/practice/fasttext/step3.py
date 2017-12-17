# _*_coding:utf-8 _*_
import logging
logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)
import fasttext
#训练模型
#classifier = fasttext.supervised("news_fasttext_train.txt","news_fasttext.model",label_prefix="__label__")

#load训练好的模型
classifier = fasttext.load_model('news_fasttext.model.bin', label_prefix='__label__')

result = classifier.test("news_fasttext_test.txt")
print result.precision
print result.recall
