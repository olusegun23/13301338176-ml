#!/usr/bin/env python
# -*-coding:utf-8-*-

"""

Implements
"""






#from pyAudioAnalysis import audioBasicIO
#from pyAudioAnalysis import audioFeatureExtraction

import audioBasicIO
import audioFeatureExtraction
import matplotlib.pyplot as plt



class MyAudio(object):

    def __init__(self):
        print


    def simple(self):
        [Fs00, x00] = audioBasicIO.readAudioFile("00.wav");
        F00 = audioFeatureExtraction.stFeatureExtraction(x00, Fs00, 0.050 * Fs00, 0.025 * Fs00);


	[Fs01, x01] = audioBasicIO.readAudioFile("01.wav");
        F01 = audioFeatureExtraction.stFeatureExtraction(x01, Fs01, 0.050 * Fs01, 0.025 * Fs01);

        plt.subplot(3, 4, 1)
        plt.plot(F00[0, :]);
        plt.plot(F01[0, :]);
        plt.xlabel('Frame no');
        plt.ylabel('ZCR');
        plt.subplot(3, 4, 2);
        plt.plot(F00[1, :]);
        plt.xlabel('Frame no');
        plt.ylabel('Energy');

        plt.subplot(3, 4, 3);
        plt.plot(F00[2, :]);
        plt.xlabel('Frame no');
        plt.ylabel('Entropy ');

        plt.subplot(3, 4, 4);
        plt.plot(F00[3, :]);
        plt.xlabel('Frame no');
        plt.ylabel('Spectral Centroid ');

        plt.subplot(3, 4, 5);
        plt.plot(F00[4, :]);
        plt.xlabel('Frame no');
        plt.ylabel('Spectral Spread ');

        plt.subplot(3, 4, 6);
        plt.plot(F00[5, :]);
        plt.xlabel('Frame no');
        plt.ylabel('Spectral Entropy ');

        plt.subplot(3, 4, 7);
        plt.plot(F00[6, :]);
        plt.xlabel('Frame no');
        plt.ylabel('Spectral Flux ');

        plt.subplot(3, 4, 8);
        plt.plot(F00[7, :]);
        plt.xlabel('Frame no');
        plt.ylabel('Spectral Rolloff ');

        plt.subplot(3, 4, 9);
        plt.plot(F00[8:20, :]);
        plt.xlabel('Frame no');
        plt.ylabel('MFCCs 9-21	');

        plt.subplot(3, 4, 10);
        plt.plot(F00[21:32, :]);
        plt.xlabel('Frame no');
        plt.ylabel('Chroma Vector22-33 ');

        plt.subplot(3, 4, 11);
        plt.plot(F00[33, :]);
        plt.xlabel('Frame no');
        plt.ylabel('Chroma Deviation ');

        plt.show()





class Mywave():
    def read(self):
        import wave
        import pylab as pl
        import numpy as np

        # 打开WAV文档
        f = wave.open(r"sweep.wav", "rb")

        # 读取格式信息
        # (nchannels, sampwidth, framerate, nframes, comptype, compname)
        params = f.getparams()
        nchannels, sampwidth, framerate, nframes = params[:4]

        # 读取波形数据
        str_data = f.readframes(nframes)
        f.close()

        # 将波形数据转换为数组
        wave_data = np.fromstring(str_data, dtype=np.short)
        wave_data.shape = -1, 2
        wave_data = wave_data.T
        time = np.arange(0, nframes) * (1.0 / framerate)

        # 绘制波形
        pl.subplot(211)
        pl.plot(time, wave_data[0])
        pl.subplot(212)
        pl.plot(time, wave_data[1], c="g")
        pl.xlabel("time (seconds)")
        pl.show()

    def write(self):
        import wave
        import numpy as np
        import scipy.signal as signal

        framerate = 44100
        time = 10

        # 产生10秒44.1kHz的100Hz - 1kHz的频率扫描波
        t = np.arange(0, time, 1.0 / framerate)
        wave_data = signal.chirp(t, 100, time, 1000, method='linear') * 10000
        wave_data = wave_data.astype(np.short)

        # 打开WAV文档
        f = wave.open(r"sweep.wav", "wb")

        # 配置声道数、量化位数和取样频率
        f.setnchannels(2)
        f.setsampwidth(2)
        f.setframerate(framerate)
        # 将wav_data转换为二进制数据写入文件
        f.writeframes(wave_data.tostring())
        f.close()




    def play(self):
        import pyaudio
        import wave

        chunk = 1024

        wf = wave.open(r"record.wav", 'rb')

        p = pyaudio.PyAudio()

        # 打开声音输出流
        stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                        channels=wf.getnchannels(),
                        rate=wf.getframerate(),
                        output=True)

        # 写声音输出流进行播放
        while True:
            data = wf.readframes(chunk)
            if data == "": break
            stream.write(data)

        stream.close()
        p.terminate()

    def record(self):
        # -*- coding: utf-8 -*-
        from pyaudio import PyAudio, paInt16
        import numpy as np
        from datetime import datetime
        import wave

        # 将data中的数据保存到名为filename的WAV文件中
        def save_wave_file(filename, data):
            wf = wave.open(filename, 'wb')
            wf.setnchannels(1)
            wf.setsampwidth(2)
            wf.setframerate(SAMPLING_RATE)
            wf.writeframes("".join(data))
            wf.close()

        NUM_SAMPLES = 2000  # pyAudio内部缓存的块的大小
        SAMPLING_RATE = 8000  # 取样频率
        LEVEL = 1500  # 声音保存的阈值
        COUNT_NUM = 20  # NUM_SAMPLES个取样之内出现COUNT_NUM个大于LEVEL的取样则记录声音
        SAVE_LENGTH = 8  # 声音记录的最小长度：SAVE_LENGTH * NUM_SAMPLES 个取样

        # 开启声音输入
        pa = PyAudio()
        stream = pa.open(format=paInt16, channels=1, rate=SAMPLING_RATE, input=True,
                         frames_per_buffer=NUM_SAMPLES)

        save_count = 0
        save_buffer = []

        while True:
            # 读入NUM_SAMPLES个取样
            string_audio_data = stream.read(NUM_SAMPLES)
            # 将读入的数据转换为数组
            audio_data = np.fromstring(string_audio_data, dtype=np.short)
            # 计算大于LEVEL的取样的个数
            large_sample_count = np.sum(audio_data > LEVEL)
            print np.max(audio_data)
            # 如果个数大于COUNT_NUM，则至少保存SAVE_LENGTH个块
            if large_sample_count > COUNT_NUM:
                save_count = SAVE_LENGTH
            else:
                save_count -= 1

            if save_count < 0:
                save_count = 0

            if save_count > 0:
                # 将要保存的数据存放到save_buffer中
                save_buffer.append(string_audio_data)
            else:
                # 将save_buffer中的数据写入WAV文件，WAV文件的文件名是保存的时刻
                if len(save_buffer) > 0:
                    filename = datetime.now().strftime("%Y-%m-%d_%H_%M_%S") + ".wav"
                    save_wave_file("record.wav", save_buffer)
                    save_buffer = []
                    print filename, "saved"

    def fft(self):
        # -*- coding: utf-8 -*-
        import numpy as np
        import pylab as pl

        sampling_rate = 8000
        fft_size = 512
        t = np.arange(0, 1.0, 1.0 / sampling_rate)
        x = np.sin(2 * np.pi * 156.25 * t) + 2 * np.sin(2 * np.pi * 234.375 * t)


        xs = x[:fft_size]
        xf = np.fft.rfft(xs) / fft_size
        freqs = np.linspace(0, sampling_rate / 2, fft_size / 2 + 1)

        xfp = 20 * np.log10(np.clip(np.abs(xf), 1e-20, 1e100))


        pl.figure(figsize=(8, 4))
        pl.subplot(211)
        pl.plot(t[:fft_size], xs)
        pl.xlabel("time(s")
        pl.title("156.25Hz 234.375Hz wave shape and freqence ")
        pl.subplot(212)



        pl.plot(freqs, xfp)
        pl.xlabel("frq(Hz)")
        pl.subplots_adjust(hspace=0.4)
        pl.show()


def nunpy_API():
    # coding:utf-8
    import numpy as np
    array = np.array([[1, 2, 3], [4, 5, 6]])
    print(array)
    print('维度：', array.ndim)  # 有几行
    print('类型：', array.shape)  # (2, 3) 有几行，几列
    print('size:', array.size)  # 总数大小
    a = np.array([2, 3, 4], dtype=np.int64)  # 定义类型 int64
    print(a.dtype)
    a = np.zeros((3, 4))  # 定义一个三行四列的矩阵，里面的只全部为0
    a = np.ones((3, 4))  # 生成一个三行四列的矩阵，值为1
    a = np.empty((3, 4))  # 生成一个三行四列的矩阵，值接近为0
    a = np.arange(1, 10, 2)  # 生成一个从1-9 步长为2的矩阵
    a = np.arange(10)  # 生成一个从0-9 的矩阵
    a = np.arange(12).reshape(3, 4)  # 生成一个3行4列的数列
    a = np.linspace(1, 10, 20)  # 生成一个1-10之间的20段线段数列
    a = np.linspace(1, 10, 6).reshape(2, 3)  # 生成一个1-10之间2行3列的6段线段
    a = np.array([10, 20, 30, 40])
    b = np.arange(4)  # 生成一个0-3之间四个数的矩阵,即0-3四个数组成的矩阵
    print(a, b)  # 打印两个矩阵
    print(a + b)  # 矩阵相加
    print(a - b)  # 矩阵相减
    print(a * b)  # 矩阵相乘
    print(a ** b)  # 矩阵乘方
    c = np.sin(a) * 10  # 对a中每个值取sin, 再乘10, con, tan都是这样的
    print(c)
    print(b < 3)  # 判断b中每个数据是不是小于3, [ True  True  True False]
    a = np.array([[1, 2], [3, 4]])
    b = np.arange(4).reshape(2, 2)
    print(a, b)

    print(a * b)  # 矩阵a和矩阵b中的每个值相乘, 相乘之后的数值组成的一个矩阵
    print(np.dot(a, b))  # 矩阵与矩阵相乘,第一个矩阵的列等于第二个矩阵的行
    print(a.dot(b))  # 跟上面的结果是一样的
    a = np.random.random((2, 4))  # 在0-1之间随机生成一个2行4列的一个矩阵
    print(a)
    print(np.sum(a))  # 矩阵里数值求和
    print('###############################')
    print(np.sum(a, axis=1))  # 矩阵里每行的求和
    print(np.sum(a, axis=0))  # 矩阵里每列的求和
    print(np.min(a))  # 矩阵里最小值
    print(np.min(a, axis=1))  # 矩阵里每行最小值
    print(np.min(a, axis=0))  # 矩阵里每列最小值
    print(np.max(a))  # 矩阵里最大值
    print(np.max(a, axis=1))  # 矩阵里每行最大值
    print(np.max(a, axis=0))  # 矩阵里每列最大值

    A = np.arange(1, 13).reshape(3, 4)  # 在1-13这12个数中，分成3行4列
    print(A)
    print(np.argmin(A))  # 求矩阵中最小值的索引 0
    print(np.argmax(A))  # 求矩阵中最大值的索引 11
    print(np.mean(A))  # 求矩阵中平均值
    print(A.mean())  # 求矩阵中平均值
    print(np.median(A))  # 求矩阵中中位数
    print(np.cumsum(A))  # 矩阵中数值累加，第一个为第一个的值，第二个为前两个值的和，第三个为前三个的和。。。
    print(np.diff(A))  # 矩阵中数值累差, 后面减前面一个的差
    print(np.nonzero(A))  # 找出矩阵中非0的数, 结果输出两个array, 第一个为行，第二个为列
    A = np.arange(13, 1, -1).reshape(3, 4)
    print(A)
    print(np.sort(A))  # 逐行从小到大排序
    print(np.transpose(A))  # 矩阵行列变换
    print(A.T)  # 矩阵行列变换，上面的简写
    print((A.T).dot(A))  # 行列变换之后的矩阵再和以前的矩阵相乘
    print(np.clip(A, 5, 10))  # 矩阵小于5的等于5, 大于10的等于10, 只保留中间部分
    print(np.mean(A, axis=1))  # 矩阵中对行计算平均值，axis=0是对列计算平均值

    A = np.arange(1, 13)
    print(A)
    print(A[3])  # 根据矩阵索引获取值，从0开始的
    A = np.arange(1, 13).reshape(3, 4)
    print(A)
    print(A[2])  # 打印出第二行数据（从0开始数）
    print(A[2][3])  # 找出矩阵第2行第3列
    print(A[2, 3])  # 找出矩阵第2行第3列
    print(A[:, :])  # 矩阵所有行所有列
    print(A[:, 1])  # 矩阵第2列所有数
    print(A[1, :])  # 矩阵第2行所有数
    print(A[1, 2:])  # 矩阵第一行第三个列及其以后的数
    for row in A:
        print(row)  # 迭代每一行
    for column in A.T:  # 想迭代列先行列变换，将列变行，再迭代
        print(column)
    print(A.flatten())  # 将三行四列的一个矩阵里的值重新放到一个新的矩阵中
    for item in A.flat:  # A.flat返回一个可迭代对象
        print(item)

    A = np.array([1, 1, 1])
    B = np.array([2, 2, 2])
    C = np.vstack((A, B))  # 将两个序列上下合并成一个矩阵
    # C = np.hstack((A,B)) #将两个序列左右合并，变成一个序列
    print(C)
    print(A[np.newaxis, :])  # 将A加一个维度，从一个序列变成由一行组成的矩阵
    print(A[:, np.newaxis])  # 将A加一个维度，从一个序列变成由一列组成的矩阵
    A = A[np.newaxis, :]  # 将A, B 分别加一个维度
    B = B[np.newaxis, :]
    print('A,B:', A, B)
    C = np.vstack((A, B))  # 将两个矩阵上下合并
    C = np.hstack((A, B))  # 将两个矩阵左右合并，这里将A、B合并成一个序列
    print(C)
    C = np.concatenate((A, B, B, A), axis=0)  # 可以进行多个矩阵合并，可以指定合并维度,axis=1指每个矩阵按行左右合并，0是每个矩阵按列上下合并
    print(C)

    A = np.arange(12).reshape((3, 4))
    print(A)
    print(np.split(A, 2, axis=1))  # 将A进行分割，分成两个array，按行等量分割，分成几块得能整除才能分割
    print(np.array_split(A, 3, axis=1))  # 将A进行分割，按行分成三个array，可以进行不等量分割
    print(np.vsplit(A, 3))  # 将A横向平均分割成3块
    print(np.hsplit(A, 2))  # 将A纵向平均分割成2块

    a = np.arange(4)
    print(a)
    b = a
    c = a
    d = b
    a[0] = 8
    print(a)
    print(d)  # 都是一样的，一个改变了其他都变
    print(b is a)  # 如果一样就是True
    d[1:3] = [11, 22]
    print(a)  # 也是一样的，等于是改的同一块内存中的数据
    # 如果想a改变，其他赋值的不变则需要深拷贝
    b = a.copy()  # 深拷贝，拷贝数据重新放到另外一块内存中
    print(a)
    b[0] = 66
    print(a, b)  # b变了，a没变


def main():
    myaudio=MyAudio()
    myaudio.simple()

    myaudio = Mywave()

    myaudio.write()
    myaudio.read()
    #all loop myaudio.record()
    #all loopmyaudio.play()
    myaudio.fft()

    #nunpy_API()



if __name__ == "__main__":
    main()

