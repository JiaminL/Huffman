#include<stdio.h> 
#include<stdlib.h>
#include<string.h>
typedef struct HuffNode{
	char data;
	struct HuffNode *lchild,*rchild;
}HuffNode,*HuffTree;
int num=0; 

FILE * openFile(char *fileName,char *openType){
	FILE *f;
	f=fopen(fileName,openType);
	if(f==NULL){ 
		printf("Can't open the file :%s",fileName); 
		exit(1);
	}
	return f;
}


HuffTree createHuffTree(FILE *huff){
	HuffTree HT;
	char type,data;
	type=fgetc(huff);
	data=fgetc(huff);
	if(feof(huff))printf("error");
	if((HT=(HuffTree)malloc(sizeof(HuffNode)))==NULL) exit(1);
	if(type=='n'){
		HT->lchild=createHuffTree(huff);
		HT->rchild=createHuffTree(huff);
	}
	else{
		HT->data=data;
		HT->lchild=HT->rchild=NULL;
	}
	return HT;
}

void deleHuffTree(HuffTree HT){
	if(HT==NULL) return;
	deleHuffTree(HT->lchild);
	deleHuffTree(HT->rchild);
	free(HT);
} 

void creatNewOriFile(char *newOriName,FILE *huff,HuffTree HT){
	int i,j;
	char cnt,newC,ch;
	HuffTree p;
	FILE *newOri;
	cnt=fgetc(huff);//补0的数目
	newOri=openFile(newOriName,"wb");
	newC=fgetc(huff);
	p=HT;
	while(!feof(huff)){
		ch=newC;
		newC=fgetc(huff);
		if(!feof(huff))
			for(i=0;i<8;){
				if(p->lchild){
					j=ch>>7;ch<<=1;
					if(j) p=p->rchild;
					else p=p->lchild;
					i++;
				}
				else{
					fprintf(newOri,"%c",p->data);
					p=HT;
				}
			}
		else{
			for(i=cnt;i<=8;){
				if(p->lchild){
					j=ch>>7;ch<<=1;
					if(j) p=p->rchild;
					else p=p->lchild;
					i++;
				}
				else{
					fprintf(newOri,"%c",p->data);
					p=HT;
				}
			}
		}
	}
	fclose(newOri);
}

int main(){
	FILE *huff;
	HuffTree HT;
	char newOriName[100];
	char huffName[100];
	scanf("%s%s",newOriName,huffName);
	huff=openFile(huffName,"rb");
	//建造huffman树 
	HT=createHuffTree(huff);
	//解压文件 
	creatNewOriFile(newOriName,huff,HT);
	
	fclose(huff);
	deleHuffTree(HT);
	
	printf("OK\n");
	fflush(stdout); 
	return 0;
	
} 
