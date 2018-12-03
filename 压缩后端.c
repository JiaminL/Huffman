#include<stdio.h> 
#include<stdlib.h>
#include<string.h>
char code[256][300];
typedef struct HuffNode{
	int times;
	char data;
	struct HuffNode *lchild,*rchild;
}HuffNode,*HuffTree;

void initTimes(int *a,int n,int init){
	int i;
	for(i=0;i<n;i++)
		a[i]=init;
}

FILE * openFile(char *fileName,char *openType){
	FILE *f;
	if((f=fopen(fileName,openType))==NULL){
		printf("Can't open the file :%s",fileName);
		exit(1);
	}
	return f;
}

HuffTree creHuffTree(int *times){
	HuffTree Node[256],p,root;
	int i,j;
	int k1,k2,t;
	for(i=0;i<256;i++){//建造叶子结点 
		p=(HuffTree)malloc(sizeof(HuffNode));
		p->data=i-128;
		p->times=times[i];
		p->lchild=p->rchild=NULL;
		Node[i]=p;
	} 
	for(i=1;i<256;i++){//建造huffuman树 ，在新构建255个结点后 ，树便建好了 
		k1=-1;
		for(j=0;j<256;j++){//给k1，k2赋初值，且k1！=k2 
			if(Node[j]!=NULL&&k1==-1){ 
				k1=j;
				continue;
			}
			if(Node[j]!=NULL){
				k2=j;
				break;
			}
		}
		for(j=0;j<256;j++){//使k1指向权值最小，k2指向权值次小 
			if(Node[j]!=NULL&&Node[j]->times<=Node[k1]->times&&j!=k1){
				k2=k1;
				k1=j;
				continue;
			}
			else if(Node[j]!=NULL&&Node[j]->times<=Node[k2]->times&&j!=k1){
				k2=j;
			}
		}
		root=(HuffTree)malloc(sizeof(HuffNode));//建立新结点，其孩子为k1，k2 指向的结点 
		root->times= Node[k1]->times+Node[k2]->times;//新结点权值为其孩子权值之和 
		root->lchild=Node[k1];
		root->rchild=Node[k2];
		Node[k1]=root;//更新结点数组 
		Node[k2]=NULL;
	}
	return root;
} 

void deleHuffTree(HuffTree HT){
	if(HT==NULL) return;
	deleHuffTree(HT->lchild);
	deleHuffTree(HT->rchild);
	free(HT);
} 

void printNode(FILE *f,HuffTree HT){
	char type,data;
	if(HT->lchild||HT->rchild){
		type='n';
		data='\0';
	}
	else{
		type='f';
		data=HT->data;
	}
	fprintf(f,"%c%c",type,data);
}

void printHuffPreOrder(FILE *f,HuffTree HT){
	printNode(f,HT);
	if(HT->lchild)printHuffPreOrder(f,HT->lchild);
	if(HT->rchild)printHuffPreOrder(f,HT->rchild);
}

void creatCode(HuffTree HT,char *s,int i){
	if(HT->lchild==NULL){
		s[i]='\0';
		strcpy(code[HT->data+128],s);
//		printf("%x: %s\n",HT->data,code[HT->data+128]);
	}
	else{
		s[i]='0';creatCode(HT->lchild,s,i+1);
		s[i]='1';creatCode(HT->rchild,s,i+1);
	}
	
}


void creatmidFile(char *midName,char *oriName,int *cntp){
	FILE *mid,*ori;
	int i,cnt=0,codeL;
	char ch,newC;
	mid=openFile(midName,"wb");
	ori=openFile(oriName,"rb");
	ch=fgetc(ori);
	while(!feof(ori)){
		codeL=strlen(code[ch+128]);
		for(i=0;i<codeL;i++){
			if(code[ch+128][i]=='0')newC=newC<<1;
			else newC=(newC<<1)|0x01;
			cnt++;
			if(cnt==8){
				fprintf(mid,"%c",newC);
				cnt=0;
			}
		}	
		ch=fgetc(ori);
	}
	if(cnt!=0){
		newC<<=(8-cnt);
		fprintf(mid,"%c",newC);
	}
	fclose(mid);
	fclose(ori);
	*cntp=cnt;
}

void creathuffFile(HuffTree HT,char *huffName,char *midName,int cnt){
	FILE *mid,*huff;
	char ch;
	mid=openFile(midName,"rb");
	huff=openFile(huffName,"wb");
	printHuffPreOrder(huff,HT);
	fprintf(huff,"%c",cnt);
	ch=fgetc(mid);
	while(!feof(mid)){
		fprintf(huff,"%c",ch);
		ch=fgetc(mid);
	}
	fclose(mid);
	fclose(huff);
}

void getTimes(char *oriName,int *times){
	char ch;
	FILE *ori; 
	initTimes(times,256,0);
	ori=openFile(oriName,"rb");
	ch=fgetc(ori);
	while(!feof(ori)){
		times[ch+128]++;
		ch=fgetc(ori);
	}
	fclose(ori);
}

int main(){
	HuffTree HT;
	int times[256];
	char oriName[100];
	char midName[100];
	char huffName[100]; 
	char s[300];
	int cnt=0;
	scanf("%s%s",oriName,huffName);
	strcpy(midName,oriName);
	strcat(midName,".mid");
	//得到频数 
	getTimes(oriName,times);
	//建造huffman树 
	HT=creHuffTree(times);
	creatCode(HT,s,0);
	//压缩文件 
	creatmidFile(midName,oriName,&cnt);
	creathuffFile(HT,huffName,midName,cnt);
	
	if(remove(midName))printf("error");
	deleHuffTree(HT);
	printf("OK\n");
	fflush(stdout); 
	return 0;
} 
