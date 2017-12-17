倚天屠龙记
pen('倚天屠龙记.txt', 'r')  
fou = open('倚天屠龙记_uft8.txt', 'w')  
line = fin.readline()  
while line:  
    newline = line.decode('GB18030').encode('utf-8')  #用GBK、GB2312都会出错  
    print newline,  
    print >> fou, newline,  
    line = fin.readline()  
fin.close()  
fou.close()  
