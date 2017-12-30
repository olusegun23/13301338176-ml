# -*-coding:utf-8-*-

import sys
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from matplotlib.pyplot import specgram
import wave

# -*-coding:utf-8-*-
import pyaudio


def play_audio():
    # 定义数据流块
    CHUNK = 10240

    # 只读方式打开wav文件
    wf = wave.open('cough_2.wav', 'rb')  # (sys.argv[1], 'rb')

    p = pyaudio.PyAudio()

    # 打开数据流
    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),
                    rate=wf.getframerate(),
                    output=True)

    # 读取数据
    data = wf.readframes(CHUNK)

    # 播放
    while data != '':
        stream.write(data)
        data = wf.readframes(CHUNK)

    # 停止数据流
    stream.stop_stream()
    stream.close()

    # 关闭 PyAudio
    p.terminate()


def record_audio(wavfile):
    """PyAudio example: Record a few seconds of audio and save to a WAVE file."""

    CHUNK = 10240
    FORMAT = pyaudio.paInt16
    CHANNELS = 2
    RATE = 44100
    RECORD_SECONDS = 5
    WAVE_OUTPUT_FILENAME = wavfile

    p = pyaudio.PyAudio()

    stream = p.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    frames_per_buffer=CHUNK)

    print("* recording")

    frames = []

    for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
        data = stream.read(CHUNK)
        frames.append(data)

    print("* done recording")

    stream.stop_stream()
    stream.close()
    p.terminate()

    wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
    wf.setnchannels(CHANNELS)
    wf.setsampwidth(p.get_sample_size(FORMAT))
    wf.setframerate(RATE)
    wf.writeframes(b''.join(frames))
    wf.close()


def play_audio(wavfile):
    CHUNK = 10240

    wf = wave.open(wavfile, 'rb')
    p = pyaudio.PyAudio()
    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),
                    rate=wf.getframerate(),
                    output=True)

    data = wf.readframes(CHUNK)

    while data != '':
        stream.write(data)
        data = wf.readframes(CHUNK)

    stream.stop_stream()
    stream.close()

    p.terminate()


def record_play():
    """
    PyAudio Example: Make a wire between input and output (i.e., record a
    few samples and play them back immediately).
    """

    import pyaudio

    CHUNK = 1024
    WIDTH = 2
    CHANNELS = 2
    RATE = 44100
    RECORD_SECONDS = 5

    p = pyaudio.PyAudio()

    stream = p.open(format=p.get_format_from_width(WIDTH),
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    output=True,
                    frames_per_buffer=CHUNK)

    print("* recording")

    for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
        data = stream.read(CHUNK)
        stream.write(data, CHUNK)

    print("* done")

    stream.stop_stream()
    stream.close()

    p.terminate()


def test_fft_wavfile():
    sampling_freq, audio = wavfile.read('cough_2.wav')

    # Print the params
    print '\nShape:', audio.shape

    print '\n len():', len(audio)
    print '\n size:', audio.size
    print 'Datatype:', audio.dtype
    audio = audio[:, 0]

    print '\n audio.shape:', audio.shape

    print 'Duration:', round(audio.shape[0] / float(sampling_freq), 10), 'seconds'

    print 'sampling_freq:', sampling_freq

    print  'type of audio:', type(audio)

    audio.tofile('cough_2-withouNormal.raw')
    print  'cough_2-withouNormal.raw size:', audio.size * 2
    print  'compue Size byte:', audio.shape[0] * 16 / 8

    old_audio = audio

    audio = audio / np.power(2, 15)

    audio.tofile('cough_2_withNormal.raw')

    time_axis = 1000 * np.arange(0, len(audio), 1) / float(sampling_freq)

    # Plot the audio signal
    plt.subplot(231)
    plt.plot(time_axis, audio, color='black')
    plt.xlabel('Time (milliseconds)')
    plt.ylabel('Amplitude')
    plt.title('Input audio signal')

    # plt.show()

    # Extract the length of the audio signal
    len_signal = len(audio)
    # Extract the half length
    len_half = np.ceil((len_signal + 1) / 2.0).astype(np.int)

    # Apply Fourier transform
    freq_signal = np.fft.fft(audio)

    # Normalization
    freq_signal = abs(freq_signal[0:len_half]) / len_signal
    # Take the square
    freq_signal **= 2
    # Extract the length of the frequency transformed signal
    len_fts = len(freq_signal)
    # Adjust the signal for even and odd cases

    if len_signal % 2:
        freq_signal[1:len_fts] *= 2
    else:
        freq_signal[1:len_fts - 1] *= 2

    # Extract the power value in dB
    signal_power = 10 * np.log10(freq_signal)
    # Build the X axis
    x_axis = np.arange(0, len_half, 1) * (sampling_freq / len_signal) / 1000.0

    # Plot the figure
    plt.subplot(232)
    plt.plot(x_axis, signal_power, color='black')
    plt.xlabel('Frequency (kHz)')
    plt.ylabel('Signal power (dB)')

    fft_size = 255
    fy = np.fft.fft(audio) / fft_size

    # 绘制三角波的FFT的前20项的振幅，由于不含下标为偶数的值均为0， 因此取
    # log之后无穷小，无法绘图，用np.clip函数设置数组值的上下限，保证绘图正确

    plt.subplot(236)
    plt.title('FFT result of ')
    plt.plot(np.clip(20 * np.log10(np.abs(fy[:20])), -120, 120), "o")
    plt.xlabel("frequency bin")
    plt.ylabel("power(dB)")

    plt.show()


def play_audio_callback():
    """PyAudio Example: Play a wave file (callback version)"""

    import pyaudio
    import wave
    import time
    import sys

    wf = wave.open('cough_2.wav', 'rb')  # (sys.argv[1], 'rb')

    p = pyaudio.PyAudio()

    def callback(in_data, frame_count, time_info, status):
        data = wf.readframes(frame_count)
        return (data, pyaudio.paContinue)

    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),
                    rate=wf.getframerate(),
                    output=True,
                    stream_callback=callback)

    stream.start_stream()

    while stream.is_active():
        time.sleep(0.1)

    stream.stop_stream()
    stream.close()
    wf.close()

    p.terminate()


def play_list():
    '''
    Module to play WAV files using PyAudio.
    Author: Vasudev Ram - http://jugad2.blogspot.com
    Adapted from the example at:
    https://people.csail.mit.edu/hubert/pyaudio/#docs
    PyAudio Example: Play a wave file.
    '''

    import pyaudio
    import wave
    import sys
    import os.path
    import time

    CHUNK_SIZE = 1024

    def play_wav(wav_filename, chunk_size=CHUNK_SIZE):
        '''
        Play (on the attached system sound device) the WAV file
        named wav_filename.
        '''

        try:
            print 'Trying to play file ' + wav_filename
            wf = wave.open(wav_filename, 'rb')
        except IOError as ioe:
            sys.stderr.write('IOError on file ' + wav_filename + '\n' + \
                             str(ioe) + '. Skipping.\n')
            return
        except EOFError as eofe:
            sys.stderr.write('EOFError on file ' + wav_filename + '\n' + \
                             str(eofe) + '. Skipping.\n')
            return

        # Instantiate PyAudio.
        p = pyaudio.PyAudio()

        # Open stream.
        stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                        channels=wf.getnchannels(),
                        rate=wf.getframerate(),
                        output=True)

        data = wf.readframes(chunk_size)
        while len(data) > 0:
            stream.write(data)
            data = wf.readframes(chunk_size)

        # Stop stream.
        stream.stop_stream()
        stream.close()

        # Close PyAudio.
        p.terminate()

    def usage():
        prog_name = os.path.basename(sys.argv[0])
        print "Usage: {} filename.wav".format(prog_name)
        print "or: {} -f wav_file_list.txt".format(prog_name)

    def main():
        lsa = len(sys.argv)
        if lsa < 2:
            usage()
            sys.exit(1)
        elif lsa == 2:
            play_wav(sys.argv[1])
        else:
            if sys.argv[1] != '-f':
                usage()
                sys.exit(1)
            with open(sys.argv[2]) as wav_list_fil:
                for wav_filename in wav_list_fil:
                    # Remove trailing newline.
                    if wav_filename[-1] == '\n':
                        wav_filename = wav_filename[:-1]
                    play_wav(wav_filename)
                    time.sleep(3)


def test_fft_pcm():
    # -*- coding:utf-8 -*-

    import array
    import os
    from matplotlib import pyplot

    fileName = 'cough_2_withNormal.raw'  # 2 channel, 16 bit per sample
    file = open(fileName, 'rb')
    base = 1 / (1 << 15)

    shortArray = array.array('h')  # int16
    size = int(os.path.getsize(fileName) / shortArray.itemsize)
    count = int(size / 2)
    shortArray.fromfile(file, size)  # faster than struct.unpack
    file.close()
    leftChannel = shortArray[::2]
    rightChannel = shortArray[1::2]

    def showPCM(leftChannel, rightChannel, start=0, end=5000):
        fig = pyplot.figure(1)

        pyplot.subplot(211)
        pyplot.title('pcm left channel [{0}-{1}] max[{2}]'.format(start, end, count))
        pyplot.plot(range(start, end), leftChannel[start:end])

        pyplot.subplot(212)
        pyplot.title('pcm right channel [{0}-{1}] max[{2}]'.format(start, end, count))
        pyplot.plot(range(start, end), rightChannel[start:end])

        pyplot.show()
        # fig.savefig('pcm.pdf') # save figure to pdf file

    showPCM(leftChannel, rightChannel, 0, count)


def test_fft_sim():
    '''
              http://old.sebug.net/paper/books/scipydoc/fft_study.html
    '''

    import numpy as np
    import pylab as pl
    x = np.random.rand(8)
    x
    xf = np.fft.fft(x)
    xf

    '''
             所谓直流信号，就是其值不随时间变化，因此我们创建一个值全为1的数组x，我们看到它的FFT结果除了下标为0的数值不为0以外，其余的都为0。(为了计算各个成分的能量多少，需要将FFT的结果除以FFT的长度)，这表示我们的时域信号是直流的，并且其成分为1。

    '''
    x1 = np.fft.ifft(xf)
    x1
    xx = np.ones(8)
    xx
    np.fft.fft(xx) / len(xx)

    '''
           下面我们产生一个周期为8个取样的正弦波，然后观察其FFT的结果。

    '''
    x = np.arange(0, 2 * np.pi, 2 * np.pi / 8)
    y = np.sin(x)
    np.fft.fft(y) / len(y)

    '''
           如何用linspace创建取样点x 要计算周期为8个取样的正弦波，就需要把0-2*pi的区间等分为8分，如果用np.linspace(0, 2*np.pi, 8)的话，产生的值为：

    '''
    np.linspace(0, 2 * np.pi, 8)
    2 * np.pi / 0.8975979

    '''
      可以看出上面用linspace只等分为7份，如果要正确使用np.linspace的话， 可以如下调用，产生9个点，并且设置endpoint=False，最终结果不包括最后的那个点：
      让我们再来看看对正弦波的FFT的计算结果吧。可以看到下标为1的复数的虚数部分为-0.5，而我们产生的正弦波的放大系数(振幅)为1，它们之间的关系是-0.5*（-2）=1。再来看一下余弦波形:
    '''
    np.linspace(0, 2 * np.pi, 9, endpoint=False)

    np.fft.fft(np.cos(x)) / len(x)

    '''
          同频率的正弦波和余弦波通过不同的系数叠加，可以产生同频率的各种相位的余弦波，因此我们可以这样来理解频域中的复数:

复数的模（绝对值）代表了此频率的余弦波的振幅
复数的辐角代表了此频率的余弦波的相位

    '''

    x = np.arange(0, 2 * np.pi, 2 * np.pi / 128)
    y = 0.3 * np.cos(x) + 0.5 * np.cos(2 * x + np.pi / 4) + 0.8 * np.cos(3 * x - np.pi / 3)
    yf = np.fft.fft(y) / len(y)
    yf[:4]
    np.angle(yf[1])
    np.abs(yf[1]), np.rad2deg(np.angle(yf[1]))
    np.abs(yf[2]), np.rad2deg(np.angle(yf[2]))
    np.abs(yf[3]), np.rad2deg(np.angle(yf[3]))

    '''
              在这个例子中我们产生了三个不同频率的余弦波，并且给他们不同的振幅和相位：

周期为128/1.0点的余弦波的相位为0， 振幅为0.3
周期为64/2.0点的余弦波的相位为45度， 振幅为0.5
周期为128/3.0点的余弦波的相位为-60度，振幅为0.8
对照yf[1], yf[2], yf[3]的复数振幅和辐角，我想你应该对FFT结果中的每个数值所表示的意思有很清楚的理解了吧。

        '''

    '''
                  合成时域信号
    前面说过通过ifft函数可以将频域信号转换回时域信号，这种转换是精确的。下面我们要写一个小程序，完成类似的事情，不过可以由用户选择只转换一部分频率回到时域信号，这样转换的结果和原始的时域信号会有误差，我们通过观察可以发现使用的频率信息越多，则此误差越小，直观地看到如何通过多个余弦波逐步逼近任意的曲线信号的:。

            '''

    # 本程序演示如何用多个正弦波合成三角波
    import numpy as np
    import pylab as pl

    # 取FFT计算的结果freqs中的前n项进行合成，返回合成结果，计算loops个周期的波形
    def fft_combine(freqs, n, loops=1):
        length = len(freqs) * loops
        data = np.zeros(length)
        index = loops * np.arange(0, length, 1.0) / length * (2 * np.pi)
        for k, p in enumerate(freqs[:n]):
            if k != 0: p *= 2  # 除去直流成分之外，其余的系数都*2
            data += np.real(p) * np.cos(k * index)  # 余弦成分的系数为实数部
            data -= np.imag(p) * np.sin(k * index)  # 正弦成分的系数为负的虚数部
        return index, data

        # 产生size点取样的三角波，其周期为1

    def triangle_wave(size):
        x = np.arange(0, 1, 1.0 / size)
        y = np.where(x < 0.5, x, 0)
        y = np.where(x >= 0.5, 1 - x, y)
        return x, y

    fft_size = 256

    # 计算三角波和其FFT
    x, y = triangle_wave(fft_size)
    fy = np.fft.fft(y) / fft_size

    # 绘制三角波的FFT的前20项的振幅，由于不含下标为偶数的值均为0， 因此取
    # log之后无穷小，无法绘图，用np.clip函数设置数组值的上下限，保证绘图正确
    pl.figure()
    pl.plot(np.clip(20 * np.log10(np.abs(fy[:20])), -120, 120), "o")
    pl.xlabel("frequency bin")
    pl.ylabel("power(dB)")
    pl.title("FFT result of triangle wave")

    # 绘制原始的三角波和用正弦波逐级合成的结果，使用取样点为x轴坐标
    pl.figure()
    pl.plot(y, label="original triangle", linewidth=2)
    for i in [0, 1, 3, 5, 7, 9]:
        index, data = fft_combine(fy, i + 1, 2)  # 计算两个周期的合成波形
        pl.plot(data, label="N=%s" % i)
    pl.legend()
    pl.title("partial Fourier series of triangle wave")
    pl.show()


def test_fft_wave(wavfile):
    import wave
    import matplotlib.pyplot as plt
    import numpy as np

    def read_wave_data(file_path):
        # open a wave file, and return a Wave_read object
        f = wave.open(file_path, "rb")
        # read the wave's format infomation,and return a tuple
        params = f.getparams()
        # get the info
        nchannels, sampwidth, framerate, nframes = params[:4]
        # Reads and returns nframes of audio, as a string of bytes.
        str_data = f.readframes(nframes)
        # close the stream
        f.close()
        # turn the wave's data to array
        wave_data = np.fromstring(str_data, dtype=np.short)
        # for the data is stereo,and format is LRLRLR...
        # shape the array to n*2(-1 means fit the y coordinate)
        wave_data.shape = -1, 2
        # transpose the data
        wave_data = wave_data.T
        # calculate the time bar
        time = np.arange(0, nframes) * (1.0 / framerate)
        return wave_data, time

    #wave_data, time = read_wave_data("output.wav")
    wave_data, time = read_wave_data(wavfile)
    # draw the wave
    plt.subplot(211)
    plt.plot(time, wave_data[0])
    plt.subplot(212)
    plt.plot(time, wave_data[1], c="g")
    plt.show()






def main():
    record_audio("output.wav")
    play_audio("output.wav")
    test_fft_wave("output.wav")

    # record_audio()
    # play_audio()
    # too much echo record_play()
    # play_audio_callback
    #test_fft_sim





if __name__ == "__main__":
    main()

    