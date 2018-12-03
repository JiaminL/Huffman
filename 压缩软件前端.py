import os,pygame,sys
import subprocess as sp
from pygame.locals import*

#            R   G   B
WHITE    = (255,255,255)
BLUE     = (  0,  0,255)
YELLOW   = (255,255,  0)
BLACK    = (  0,  0,  0)
GARY     = (235,235,235)

FPS=30
WINDOWWIDTH=1200
WINDOWHEIGHT=700
PARTWIDTH=WINDOWWIDTH/3
TEXTSIZE1=50
TEXTSIZE2=25
TEXTSIZE3=35
GAPSIZE=5
BUTTOMWIDTH=100
BUTTOMHEIGHT=40
TEXTBEGIN=GAPSIZE*3+TEXTSIZE1
ORIFOLDER="huffman实验测试集"
ZIPFOLDER="huffman实验压缩文件"
UNZIPFOLDER="huffman实验解压文件"
PARTNAME=("原文件","压缩文件","解压文件")
EXENAME=("./压缩后端","./解压后端")

def sedInform(exeName,oriFile,midFile,huffFile):
    p = sp.Popen(exeName, stdin=sp.PIPE, stdout=sp.PIPE)
    while True: 
        try:
            data = "{}\n{}\n{}\n".format(oriFile, midFile, huffFile).encode("ascii") 
            p.stdin.write(data)
            p.stdin.flush()
        except:
            print("there is some errors when send the name of files to c programe")

def getFileName():
    F=[]
    for fileDir in (ORIFOLDER,ZIPFOLDER,UNZIPFOLDER):
        for root,dirs,files in os.walk(fileDir):
            dele=[]
            if fileDir==ZIPFOLDER:
                for path in files:
                    if os.path.splitext(path)[1]!=".huff":
                        dele.append(path)
            elif fileDir==ORIFOLDER:
                for path in files:
                    if os.path.splitext(path)[1]==".mid":
                        dele.append(path)
            for path in dele:
                    files.remove(path)
            F.append(files)
    return F

def newFolder():
    path=os.path.abspath("")
    for folerName in (ORIFOLDER,ZIPFOLDER,UNZIPFOLDER):
        try:
            os.path.join(path,folerName)
            os.mkdir(path+"\\"+folerName)
        except:
            continue
        
def drawText(Text,size,left,top,textcolor,boxcolor):
    fontObj=pygame.font.Font('simkai.ttf',size)
    textSurfaceObj=fontObj.render(Text,True,textcolor,boxcolor)
    textRectObj=textSurfaceObj.get_rect()
    textRectObj.topleft=(left,top)
    DISPLAYSURF.blit(textSurfaceObj,textRectObj)
        
def drawWindow(files):
    DISPLAYSURF.fill(WHITE)
    for i in range(3):
        x=i*PARTWIDTH
        pygame.draw.line(DISPLAYSURF,BLUE,(x,0),(x,WINDOWHEIGHT),2)
        x+=GAPSIZE
        drawText(PARTNAME[i],TEXTSIZE1,x,GAPSIZE,BLACK,WHITE)
        y=TEXTBEGIN
        for fileName in files[i]:
            drawText(fileName,TEXTSIZE2,x,y,BLACK,WHITE)
            y+=TEXTSIZE2+GAPSIZE

def getBoxAtPixel(mousex,mousey):
    boxx=int(mousex/PARTWIDTH)
    boxy=int((mousey-TEXTBEGIN)/(TEXTSIZE2+GAPSIZE))
    return (boxx,boxy)

def getChoice():
    mousex=0
    mousey=0
    while True:
        files=getFileName()
        oriFile,zipFile,unzipFile=files
        fileLength=(len(oriFile),len(zipFile),len(unzipFile))
        mouseClicked=False
        drawWindow(files)
        for event in pygame.event.get():
            if event.type==QUIT or (event.type==KEYUP and event.key==K_ESCAPE):
                pygame.quit()
                sys.exit()
            elif event.type==MOUSEMOTION:
                mousex,mousey=event.pos
            elif event.type==MOUSEBUTTONUP:
                mousex,mousey=event.pos
                mouseClicked=True
        myChoice=getBoxAtPixel(mousex,mousey)
        if myChoice[0]<2 and myChoice[1]>=0 and myChoice[1]<fileLength[myChoice[0]]:
            if myChoice[0]:text="解压"
            else:text="压缩"
            left,top=(myChoice[0]*PARTWIDTH,TEXTBEGIN+(TEXTSIZE2+GAPSIZE)*myChoice[1])
            pygame.draw.rect(DISPLAYSURF,YELLOW,(left,top-GAPSIZE/2,PARTWIDTH,TEXTSIZE2+GAPSIZE),4)
            drawText(text,TEXTSIZE2,(myChoice[0]+1)*PARTWIDTH-2*TEXTSIZE2,top,YELLOW,WHITE)
            if mouseClicked:
                return myChoice
        pygame.display.update()

def run(path1,choise):
    files=getFileName()
    if choise[0]==0:
        text="压缩“"+files[choise[0]][choise[1]]+"”中……请耐心等待"
        oriname=path1+"\\"+ORIFOLDER+"\\"+files[choise[0]][choise[1]]
        huffname=path1+"\\"+ZIPFOLDER+"\\"+files[choise[0]][choise[1]]+".huff"
    else:
        path=files[choise[0]][choise[1]]
        text="解压“"+path+"”中……请耐心等待"
        oriname=path1+"\\"+UNZIPFOLDER+"\\new"+os.path.splitext(path)[0]
        huffname=path1+"\\"+ZIPFOLDER+"\\"+files[choise[0]][choise[1]]
    try:
        p = sp.Popen(EXENAME[choise[0]], stdin=sp.PIPE, stdout=sp.PIPE)
        data = "{}\n{}\n".format(oriname, huffname).encode("gbk")
        drawText(text,TEXTSIZE3,GAPSIZE,WINDOWHEIGHT-TEXTSIZE3-GAPSIZE,BLACK,GARY)
        pygame.display.update()
        p.stdin.write(data)
        p.stdin.flush()
        s=p.stdout.readline()
    except:
        print("error run")
        print( "{}\n{}\n".format(oriname, huffname))
        

    
def main():
    global DISPLAYSURF
    path=os.path.abspath("")
    newFolder()
    pygame.init()
    DISPLAYSURF=pygame.display.set_mode((WINDOWWIDTH,WINDOWHEIGHT))
    pygame.display.set_caption('压缩软件')
    while True:
        files=getFileName()
        drawWindow(files)
        for event in pygame.event.get():
            if event.type==QUIT:
                pygame.quit()
                sys.exit()
        choise=getChoice()
        run(path,choise)
        pygame.display.update()

    
if __name__=='__main__':
    main()
