# UART Tx/Rx demo
import tkinter as tk
from tkinter import ttk
import serial
import threading
import time

# A simple Information Window
class InformWindow:
    def __init__(self,informStr):
        self.window = tk.Tk()
        self.window.title("Information")
        self.window.geometry("220x60")
        label = tk.Label(self.window, text=informStr)
        buttonOK = tk.Button(self.window,text="OK",command=self.processButtonOK)
        label.pack(side = tk.TOP)
        buttonOK.pack(side = tk.BOTTOM)
        self.window.mainloop()

    def processButtonOK(self):
        self.window.destroy()
        
# class bmp:
#     """ bmp data structure """

#     def __init__(self, w=320, h=240):
#         self.w = w
#         self.h = h

#     def calc_data_size (self):
#         if((self.w*3)%4 == 0):
#             self.dataSize = self.w * 3 * self.h
#         else:
#             self.dataSize = (((self.w * 3) // 4 + 1) * 4) * self.h

#         self.fileSize = self.dataSize + 54

#     def conv2byte(self, l, num, len):
#         tmp = num
#         for i in range(len):
#             l.append(tmp & 0x000000ff)
#             tmp >>= 8

#     def gen_bmp_header (self):
#         self.calc_data_size();
#         self.bmp_header = [0x42, 0x4d]
#         self.conv2byte(self.bmp_header, self.fileSize, 4) #file size
#         self.conv2byte(self.bmp_header, 0, 2)
#         self.conv2byte(self.bmp_header, 0, 2)
#         self.conv2byte(self.bmp_header, 54, 4) #rgb data offset
#         self.conv2byte(self.bmp_header, 40, 4) #info block size
#         self.conv2byte(self.bmp_header, self.w, 4)
#         self.conv2byte(self.bmp_header, self.h, 4)
#         self.conv2byte(self.bmp_header, 1, 2)
#         self.conv2byte(self.bmp_header, 24, 2) #888
#         self.conv2byte(self.bmp_header, 0, 4)  #no compression
#         self.conv2byte(self.bmp_header, self.dataSize, 4) #rgb data size
#         self.conv2byte(self.bmp_header, 0, 4)
#         self.conv2byte(self.bmp_header, 0, 4)
#         self.conv2byte(self.bmp_header, 0, 4)
#         self.conv2byte(self.bmp_header, 0, 4)
 
#     def print_bmp_header (self):
#         length = len(self.bmp_header)
#         for i in range(length):
#             print("{:0>2x}".format(self.bmp_header[i]), end=' ')
#             if i%16 == 15:
#                 print('')
#         print('')

        
class mainGUI:
    def __init__(self):
        window = tk.Tk()
        window.title("GUI UART Tx/Rx Demo")
        self.uartState = False # is uart open or not

        # a frame contains COM's information, and start/stop button
        frame_COMinf = tk.Frame(window)
        frame_COMinf.grid(row = 1, column = 1)

        labelCOM = tk.Label(frame_COMinf,text="COMx: ")
        self.COM = tk.StringVar(value = "COM9")
        ertryCOM = tk.Entry(frame_COMinf, textvariable = self.COM)
        labelCOM.grid(row = 1, column = 1, padx = 5, pady = 3)
        ertryCOM.grid(row = 1, column = 2, padx = 5, pady = 3)

        labelBaudrate = tk.Label(frame_COMinf,text="Baudrate: ")
        self.Baudrate = tk.IntVar(value = 921600)
        ertryBaudrate = tk.Entry(frame_COMinf, textvariable = self.Baudrate)
        labelBaudrate.grid(row = 1, column = 3, padx = 5, pady = 3)
        ertryBaudrate.grid(row = 1, column = 4, padx = 5, pady = 3)

        labelParity = tk.Label(frame_COMinf,text="Parity: ")
        self.Parity = tk.StringVar(value ="NONE")
        comboParity = ttk.Combobox(frame_COMinf, width = 17, textvariable=self.Parity)
        comboParity["values"] = ("ODD","NONE","EVEN","MARK","SPACE")
        comboParity["state"] = "readonly"
        labelParity.grid(row = 2, column = 1, padx = 5, pady = 3)
        comboParity.grid(row = 2, column = 2, padx = 5, pady = 3)

        labelStopbits = tk.Label(frame_COMinf,text="Stopbits: ")
        self.Stopbits = tk.StringVar(value ="1")
        comboStopbits = ttk.Combobox(frame_COMinf, width = 17, textvariable=self.Stopbits)
        comboStopbits["values"] = ("1","1.5","2")
        comboStopbits["state"] = "readonly"
        labelStopbits.grid(row = 2, column = 3, padx = 5, pady = 3)
        comboStopbits.grid(row = 2, column = 4, padx = 5, pady = 3)
        
        self.buttonSS = tk.Button(frame_COMinf, text = "Start", command = self.processButtonSS)
        self.buttonSS.grid(row = 3, column = 4, padx = 5, pady = 3, sticky = tk.E)

        # serial object
        self.ser = serial.Serial()
        # serial read threading
        self.ReadUARTThread = threading.Thread(target=self.ReadUART)
        self.ReadUARTThread.start()

        frameRecv = tk.Frame(window)
        frameRecv.grid(row = 2, column = 1)
        labelOutText = tk.Label(frameRecv,text="Received Data:")
        labelOutText.grid(row = 1, column = 1, padx = 3, pady = 2, sticky = tk.W)
        frameRecvSon = tk.Frame(frameRecv)
        frameRecvSon.grid(row = 2, column =1)
        scrollbarRecv = tk.Scrollbar(frameRecvSon)
        scrollbarRecv.pack(side = tk.RIGHT, fill = tk.Y)
        self.OutputText = tk.Text(frameRecvSon, wrap = tk.WORD, width = 60, height = 20, yscrollcommand = scrollbarRecv.set)
        self.OutputText.pack()

        frameTrans = tk.Frame(window)
        frameTrans.grid(row = 3, column = 1)
        labelInText = tk.Label(frameTrans,text="To Transmit Data:")
        labelInText.grid(row = 1, column = 1, padx = 3, pady = 2, sticky = tk.W)
        frameTransSon = tk.Frame(frameTrans)
        frameTransSon.grid(row = 2, column =1)
        scrollbarTrans = tk.Scrollbar(frameTransSon)
        scrollbarTrans.pack(side = tk.RIGHT, fill = tk.Y)
        self.InputText = tk.Text(frameTransSon, wrap = tk.WORD, width = 60, height = 5, yscrollcommand = scrollbarTrans.set)
        self.InputText.pack()
        self.buttonSend = tk.Button(frameTrans, text = "Send", command = self.processButtonSend)
        self.buttonSend.grid(row = 3, column = 1, padx = 5, pady = 3, sticky = tk.E)
      
        self.buttonbuffer = tk.Button(frameTrans, text = "collect buffer", command = self.processButtonbuffer)
        self.buttonbuffer.grid(row = 3, column = 2, padx = 5, pady = 3, sticky = tk.E)

      
        window.mainloop()

    def processButtonSS(self):
        # print(self.Parity.get())
        if (self.uartState):
            self.ser.close()
            self.buttonSS["text"] = "Start"
            self.uartState = False
        else:
            # restart serial port
            self.ser.port = self.COM.get()
            self.ser.baudrate = self.Baudrate.get()
            
            strParity = self.Parity.get()
            if (strParity=="NONE"):
                self.ser.parity = serial.PARITY_NONE
            elif(strParity=="ODD"):
                self.ser.parity = serial.PARITY_ODD
            elif(strParity=="EVEN"):
                self.ser.parity = serial.PARITY_EVEN
            elif(strParity=="MARK"):
                self.ser.parity = serial.PARITY_MARK
            elif(strParity=="SPACE"):
                self.ser.parity = serial.PARITY_SPACE
                
            strStopbits = self.Stopbits.get()
            if (strStopbits == "1"):
                self.ser.stopbits = serial.STOPBITS_ONE
            elif (strStopbits == "1.5"):
                self.ser.stopbits = serial.STOPBITS_ONE_POINT_FIVE
            elif (strStopbits == "2"):
                self.ser.stopbits = serial.STOPBITS_TWO
            
            try:
                self.ser.open()
            except:
                infromStr = "Can't open "+self.ser.port
                InformWindow(infromStr)
            
            if (self.ser.isOpen()): # open success
                self.buttonSS["text"] = "Stop"
                self.uartState = True

    def processButtonSend(self):
        if (self.uartState):
            strToSend = self.InputText.get(1.0,tk.END)
            str1='ff 73 0D 0A'
            
            
            bytesToSend = bytes.fromhex(strToSend)
            print(bytesToSend)
            self.ser.write(bytesToSend)
            
        else:
            infromStr = "Not In Connect!"
            InformWindow(infromStr)
            
    def processButtonbuffer(self):
        if (self.uartState):
            
           buffer=self.ser.read_all()
           #RGB565 16 HEADER
           str3='42 4D 36 58 02 00 00 00 00 00 36 00 00 00 28 00 00 00 40 01 00 00 F0 00 00 00 01 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00'            
           #RGB565 24 HEADER
           str2='42 4d 36 84 03 00 00 00 00 00 36 00 00 00 28 00 00 00 40 01 00 00 f0 00 00 00 01 00 18 00 00 00 00 00 00 84 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00' 
           
           wenjian=open('s.bmp','wb')
           wenjian.write(bytes.fromhex(str3))
           
           # for i,j in zip(buffer[0::2],buffer[1::2]):
           #     # print(i,j)
           #     # tmp=i>>3
           #     # tmp|=(j&224)
           #     # print(tmp)
           #     wenjian.write(bytes([j]))
           #     # tmp=j<<3&255
           #     # tmp|=(i&7)
           #     # print(tmp)
           #     wenjian.write(bytes([i]))
           #     # wenjian.write(buffer)
           
               
           wenjian.write(buffer)

           wenjian.close()
           

           
           
    
    
    def ReadUART(self):
        # print("Threading...")
        buffer=b''
        while True:
            # print('1') 
            time.sleep(0.1)
            if (self.uartState):
                # try:
                    # ch = self.ser.read().decode(encoding='ascii')
                    ch=self.ser.read_all()
                    
                    buffer=buffer+ch
                    if(len(buffer)>=153600):
                        print(len(buffer))
                        str3='42 4D 36 58 02 00 00 00 00 00 36 00 00 00 28 00 00 00 40 01 00 00 F0 00 00 00 01 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00'            
                        
                        str2='42 4d 36 84 03 00 00 00 00 00 36 00 00 00 28 00 00 00 40 01 00 00 f0 00 00 00 01 00 18 00 00 00 00 00 00 84 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00' 
                        s=open('s.bmp','wb')
                        s2=open('s2.bmp','wb')
                        s_r=open('s_r.bmp','wb')
                        s_g=open('s_g.bmp','wb')
                        s_b=open('s_b.bmp','wb')
                        
                        s.write(bytes.fromhex(str2))
                        s2.write(bytes.fromhex(str2))
                        s_r.write(bytes.fromhex(str2))
                        s_g.write(bytes.fromhex(str2))
                        s_b.write(bytes.fromhex(str2))
                        
                        for i,j in zip(buffer[0::2],buffer[1::2]):
                            #b
                            s.write(bytes([(j&31)<<3]))
                            #g
                            s.write(bytes([((i&7)<<5)+((j&224)>>3)]))
                            #r
                            s.write(bytes([i&248]))
                            
                            s2.write(bytes([i]))     
                            s2.write(bytes([j]))
                     
                            s_b.write(bytes([(j&31)<<3]))     
                            s_b.write(bytes([0]))
                            s_b.write(bytes([0]))
                            
                            s_g.write(bytes([0]))     
                            s_g.write(bytes([((i&7)<<5)+((j&224)>>3)]))
                            s_g.write(bytes([0]))     
                            
                            s_r.write(bytes([0]))     
                            s_r.write(bytes([0]))
                            s_r.write(bytes([i&248]))
    

                        s.close()
                        s2.close()
                        s_r.close()
                        s_g.close()
                        s_b.close()
                        
                        buffer=b''
                        print('3')

                    # print(ch,end='')
                    # self.OutputText.insert(tk.END,ch)
                # except:
                #     infromStr = "Something wrong in receiving."
                #     InformWindow(infromStr)
                #     self.ser.close() # close the serial when catch exception
                #     self.buttonSS["text"] = "Start"
                #     self.uartState = False
             

mainGUI()
