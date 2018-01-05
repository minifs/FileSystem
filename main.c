#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "block.h"
#include "inode_entry.h"
#include "file.h"


char PATH[100];
char tempPATH[100];

int initial_Filesystem(){
	
	if(load_filesystem("mini-FileSystem")<=-1){
		int ret=create_filesystem("mini-FileSystem");
		if(ret<0)
		    return ret;
		strcpy(PATH,dir_init());
		return 0;
	}
	strcpy(PATH,dir_init());    
    
	//strcpy(PATH,"/dir2/dor3");
    return 0; 

}

void parse_string(char *string, char **argv, int *argc){
	int count=0;
	
	count=strlen(string)-1;
	(*argc)=0;
    for(int i=0;i<=count;i++){
            if(string[i]=='/')
                string[i]='\0';     
    }
    
    if(string[0]!='\0'){
    	argv[(*argc)]=&string[0];
    	(*argc)++;
    }
	else if(string[0]=='\0'&&string[1]=='\0');    
    else if(string[0]=='\0'&&string[1]!='\0'){
    	argv[(*argc)]=&string[1];
    	(*argc)++;
    }
       
	

    for(int i=1;i<=count-1;i++){
        if(string[i]=='\0'&&string[i+1]!='\0'){
            argv[(*argc)]=&string[i+1];
            (*argc)++;
                
       }
    }
	
	
    
}



/*
 * 相對路徑轉絕對路徑
 * argument:
 *     char *relative :相對路徑 
 *	   char *absolute :絕對路徑 
*/ 
void relative_to_absolute_path(char *relative, char *absolute){
	
	if(relative[0]!='/'){
		char *argv[64];
	    int argc=0;
	    int k=0;
	
	    strcpy(tempPATH,PATH);
	    //printf("%s %s\n",tempStack[0],tempStack[1]);
	    parse_string(relative,argv,&argc);
	    //printf("%s %s %s\n",argv[0],argv[1],argv[2]);
	
	    for(int i=0;i<=argc-1;i++){
		    if(strcmp(argv[i],".")==0)
		        continue;
		    else if(strcmp(argv[i],"..")==0){
			    for(int k=strlen(tempPATH)-1;k>=0;k--){
				    if(tempPATH[k]=='/'&&k!=0){
					    tempPATH[k]='\0';
					    break;
				    }
				    else if(tempPATH[k]=='/'&&k==0){//已經切到root目錄了
				    	tempPATH[k+1]='\0';
					    break;
					}
				
		    	}
		    }	
		    else{
		    	if(tempPATH[strlen(tempPATH)-1]!='/')
			        strcat(tempPATH,"/");
			    strcat(tempPATH,argv[i]);
	    	}
    	}
	
	    strcpy(absolute,tempPATH);
	}
	else
	    strcpy(absolute,relative); 
	
	
	
	
}



int main(){
	char inputcmd[100];
	char absolutePath[100];
	char *argv[64];
	int count=0,argc=0;
	//char tempReturn[100000]="dir1\ndate.txt\ncat.mp3\n";
	char tempReturn[100000];
	
	//--------testcase---------
	char ls_return[1000];
	//-------------------------
    if(initial_Filesystem()<0){
		printf("FileSystem create/load error\n");
		return 0;
	}
    //relative_to_absolute_path(inputcmd,absolutePath);
    //printf("%s",absolutePath);
    //stack_to_path(currentPath,current_Path_Stack,top_of_stack);
    //printf("%s",currentPath);
    
    printf("           _       _       _____ _ _      ____            _                 \n");
	printf(" _ __ ___ (_)_ __ (_)     |  ___(_) | ___/ ___| _   _ ___| |_ ___ _ __ ___  \n");
	printf("| '_ ` _ \| | '_ \| |_____| |_  | | |/ _ \___ \| | | / __| __/ _ \ '_ ` _ \ \n");
	printf("| | | | | | | | | | |_____|  _| | | |  __/___) | |_| \__ \ ||  __/ | | | | |\n");
	printf("|_| |_| |_|_|_| |_|_|     |_|   |_|_|\___|____/ \__, |___/\__\___|_| |_| |_|\n");
	printf("                                                |___/\n");
	
	while(1){
		count=0;
		argc=0;
		strcpy(tempReturn,"");
		//strcpy(tempReturn,"dir1\ndate.txt\ncat.mp3\n");
		
		printf(">");
        fgets(inputcmd,sizeof(inputcmd),stdin);
        if(strcmp(inputcmd,"exit\n")==0||strcmp(inputcmd,"q\n")==0)
         break;
        

        count=strlen(inputcmd)-1;
        for(int i=0;i<=count;i++){//用'\0'替換' ','\n'，目的是要把指令和所有參數都變成字串
              if(inputcmd[i]==' '||inputcmd[i]=='\n')
                 inputcmd[i]='\0';     
        }
        argv[argc]=&inputcmd[0];
        argc++;
        for(int i=1;i<=count-1;i++){//把每個字串的位址存到argv
              if(inputcmd[i]=='\0'&&inputcmd[i+1]!='\0'){
                argv[argc]=&inputcmd[i+1];
                argc++;
                
                     
              }
        }
        
        //for(int i=0;i<=argc-1;i++)
        //    printf("%s ",argv[i]);
        //printf("\n");
        
        
        /*
         * Command API
        */
        if(strcmp(argv[0],"ls")==0){
        	strcpy(tempReturn,".");
        	strcat(tempReturn,":\n");
        	dir_ls(ls_return,PATH);
        	strcat(tempReturn,ls_return);
        	//strcat(tempReturn,"dir1\ndate.txt\ncat.mp3\n");
        	if(argc==1){

        		printf("%s",tempReturn);
			}
			else if(argc>=2){
				
				for(int i=1;i<=argc-1;i++){
					if(strcmp(argv[i],">")==0&&(i+1)<=argc-1){
						relative_to_absolute_path(argv[i+1],absolutePath);
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}

					}
					else if(strcmp(argv[i],">")==0&&i==argc-1){
						printf("write file error\n");
						break;
					}
					else if(strcmp(argv[i],">")!=0&&i==1){
						relative_to_absolute_path(argv[i],absolutePath);
						if(dir_change(absolutePath)<0){
						//if(0){
							strcpy(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							strcat(tempReturn,"No such file or directory.\n");
						}    
				        else{
							strcpy(tempReturn,argv[i]);
        	                strcat(tempReturn,":\n");
        	                dir_ls(ls_return,absolutePath);
        	                strcat(tempReturn,ls_return);
        	                //strcat(tempReturn,"dir1\ndate.txt\ncat.mp3\n");
						}
					}
					else if(strcmp(argv[i],">")!=0){
						relative_to_absolute_path(argv[i],absolutePath);
						if(dir_change(absolutePath)<0){
						//if(0){
							strcat(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							strcat(tempReturn,"No such file or directory.\n");
						}    
				        else{
							strcat(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							dir_ls(ls_return,absolutePath);
							strcat(tempReturn,ls_return);
							//strcat(tempReturn,"dir1\ndate.txt\ncat.mp3\n");
						}
					}

                     if(i==argc-1){
						 if(strcmp(tempReturn,"")!=0)
						     printf("%s",tempReturn);
					 }
				}
				
			}
        	
        }
        else if(strcmp(argv[0],"pwd")==0){
        	printf("%s\n",PATH);
			
        	
		}
		else if(strcmp(argv[0],"cd")==0){
			if(argc==1)//回根目錄
			    strcpy(PATH,"/");
			else{
			
				/*if(argv[1][0]=='\"'&&argv[1][strlen(argv[1])-1]=='\"'){//檔名如果是""括起來，去掉""的部分 
				    argv[1][strlen(argv[1])-1]='\0';
				    argv[1]=&argv[1][1];
			    } */ 
			    
			    for(int i=1;i<=argc-1;i++){
					if(strcmp(argv[i],">")==0&&(i+1)<=argc-1){
						relative_to_absolute_path(argv[i+1],absolutePath);
						
						if(i==1)
						    strcpy(PATH,"/");
						    
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}

					}
					else if(strcmp(argv[i],">")==0&&i==argc-1){
						printf("write file error\n");
						break;
					}
					else if(strcmp(argv[i],">")!=0&&i==1){
						relative_to_absolute_path(argv[i],absolutePath);
						if(dir_change(absolutePath)<0){
						//if(0){
							strcpy(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							strcat(tempReturn,"No such file or directory.\n");
						}    
				        else{
							strcpy(PATH,absolutePath);
						}
					}

                     if(i==argc-1){
						 if(strcmp(tempReturn,"")!=0)
						     printf("%s",tempReturn);
					 }
                         
				}
				
			    
				
                
			} 
			
			
				 
			
		    
		}
		else if(strcmp(argv[0],"cat")==0){
			if(argc==1)
			    printf("wrong format\n");
            else{
				for(int i=1;i<=argc-1;i++){
					if(strcmp(argv[i],">")==0&&i==1&&(i+1)<=argc-1){//ex:cat > test.txt
						relative_to_absolute_path(argv[i+1],absolutePath);
						strcpy(tempReturn,"wrong format\n");
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}
					}
					else if(strcmp(argv[i],">")==0&&(i+1)<=argc-1){
						relative_to_absolute_path(argv[i+1],absolutePath);
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}

					}
					else if(strcmp(argv[i],">")==0&&i==argc-1){
						printf("write file error\n");
						break;
					}
					else if(strcmp(argv[i],">")!=0){
						relative_to_absolute_path(argv[i],absolutePath);
						if(search_file(absolutePath)==-1){
						//if(0){
							strcat(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							strcat(tempReturn,"No such file or directory.\n");
						}    
				        else{
							void *buf=NULL;
							printf("[read_file_by_path]path:%s buf=%s\n",absolutePath,buf);
							int ret=read_file_by_path(absolutePath,&buf);
							printf("[read_file_by_path]ret:%d buf=%s\n",ret,(char*)buf);
							
							strcat(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							strcat(tempReturn,(char*)buf);
							//strcat(tempReturn,"hellow world\n");
							//strcat(tempReturn,"\n");
						}
					}
					
					if(i==argc-1){
						 if(strcmp(tempReturn,"")!=0)
						     printf("%s",tempReturn);
					 }

				}
			}
		}
		else if(strcmp(argv[0],"rm")==0){//remove file
			if(argc==1)
			    printf("wrong format\n");
            else{
				for(int i=1;i<=argc-1;i++){
					if(strcmp(argv[i],">")==0&&i==1&&(i+1)<=argc-1){//ex:rm > test.txt
						relative_to_absolute_path(argv[i+1],absolutePath);
						strcpy(tempReturn,"wrong format\n");
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}
					}
					else if(strcmp(argv[i],">")==0&&(i+1)<=argc-1){
						relative_to_absolute_path(argv[i+1],absolutePath);
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}

					}
					else if(strcmp(argv[i],">")==0&&i==argc-1){
						printf("write file error\n");
						break;
					}
					else if(strcmp(argv[i],">")!=0){
						relative_to_absolute_path(argv[i],absolutePath);
						if(search_file(absolutePath)==-1){
						//if(0){
							strcat(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							strcat(tempReturn,"No such file or directory.\n");
						}    
				        else{
							printf("[delete_file_by_path]path:%s\n",absolutePath);
							int ret=delete_file_by_path(absolutePath);
							printf("[delete_file_by_path]ret:%d\n",ret);
						}
					}
					
					if(i==argc-1){
						 if(strcmp(tempReturn,"")!=0)
						     printf("%s",tempReturn);
					 }

				}
			}
			
		}
		else if(strcmp(argv[0],"rmdir")==0){//remove directory
			if(argc==1)
			    printf("wrong format\n");
            else{
				for(int i=1;i<=argc-1;i++){
					if(strcmp(argv[i],">")==0&&i==1&&(i+1)<=argc-1){//ex:rmdir > test.txt
						relative_to_absolute_path(argv[i+1],absolutePath);
						strcpy(tempReturn,"wrong format\n");
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}
					}
					else if(strcmp(argv[i],">")==0&&(i+1)<=argc-1){
						relative_to_absolute_path(argv[i+1],absolutePath);
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}

					}
					else if(strcmp(argv[i],">")==0&&i==argc-1){
						printf("write file error\n");
						break;
					}
					else if(strcmp(argv[i],">")!=0){
						relative_to_absolute_path(argv[i],absolutePath);
						if(dir_change(absolutePath)<0){
						//if(0){
							strcat(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							strcat(tempReturn,"No such file or directory.\n");
						}    
				        else{
							for(int k=strlen(absolutePath)-1;k>=0;k--){
				                if(absolutePath[k]=='/'&&k!=0){
					                absolutePath[k]='\0';
					                
					                printf("[dir_delete]pwd:%s foldername=%s\n",absolutePath,&absolutePath[k+1]);
					                int ret=dir_delete(absolutePath,&absolutePath[k+1]);
					                printf("[dir_delete]ret:%d\n",ret);
					                
					                break;
				                }
				                else if(absolutePath[k]=='/'&&k==0){//已經切到root目錄了
				    	            absolutePath[k]='\0';
				    	            
				    	            printf("[dir_delete]pwd:%s foldername=%s\n","/",&absolutePath[k+1]);
				    	            int ret=dir_delete("/",&absolutePath[k+1]);
				    	            printf("[dir_delete]ret:%d\n",ret);
					                break;
					            }
				
		    	             }
							
						}
					}
					
					if(i==argc-1){
						 if(strcmp(tempReturn,"")!=0)
						     printf("%s",tempReturn);
					 }

				}
			}
		}
		else if(strcmp(argv[0],"cp")==0){//不支援cp整個目錄
			if(argc<=2)
			    printf("wrong format\n");
            else{
				int cpTarget=0,m;
				char target[100];
				char tempName[100]="";
				for(m=1;m<=argc-1;m++){
					if(strcmp(argv[m],">")==0){
						cpTarget=m-1;
						break;
					}
				}
				if(m==argc)
				    cpTarget=argc-1;
				
				//------target process-----
				relative_to_absolute_path(argv[cpTarget],absolutePath);
			    if(dir_change(absolutePath)==0){
			    //if(1){
					
					strcpy(target,absolutePath);
				}
				else{
					for(int k=strlen(absolutePath)-1;k>=0;k--){
				                if(absolutePath[k]=='/'&&k!=0){
					                absolutePath[k]='\0';
					                if(dir_change(absolutePath)==0){
					                //if(1){
					
					                    strcpy(target,absolutePath);
					                    strcpy(tempName,&absolutePath[k+1]);
				                    }
				                    else{
										strcpy(target,"");
									}
					                break;
				                }
				                else if(absolutePath[k]=='/'&&k==0){//已經切到root目錄了
				    	            strcpy(target,"/");
				    	            strcpy(tempName,&absolutePath[k+1]);
					                break;
					            }
				
		    	             }
				}
				
				if(strcmp(target,"")==0){
					strcat(tempReturn,argv[cpTarget]);
					strcat(tempReturn,":\n");
					strcat(tempReturn,"No such file or directory.\n");
				}
				    
			        
				//------------------------    
				for(int i=1;i<=argc-1;i++){
					if(strcmp(argv[i],">")==0&&i==1&&(i+1)<=argc-1){//ex:cp > test.txt
						relative_to_absolute_path(argv[i+1],absolutePath);
						strcpy(tempReturn,"wrong format\n");
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}
					}
					else if(strcmp(argv[2],">")==0){
						printf("wrong format\n");
						break;
					}
					else if(strcmp(argv[i],">")==0&&(i+1)<=argc-1){
						relative_to_absolute_path(argv[i+1],absolutePath);
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}

					}
					else if(strcmp(argv[i],">")==0&&i==argc-1){
						printf("write file error\n");
						break;
					}
					else if(strcmp(argv[i],">")!=0&&i!=argc-1&&strcmp(argv[i+1],">")!=0){//避免target複製到target
						relative_to_absolute_path(argv[i],absolutePath);
						if(search_file(absolutePath)==-1){//找不到
						//if(0){
							strcat(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							strcat(tempReturn,"No such file or directory.\n");
							
						}    
				        else if(search_file(absolutePath)==0){//找到
				        //else if(1){
							if(cpTarget>=3){//多檔案複製
								if(strcmp(target,"")!=0){
								    int len;
								    void *buf=NULL;
								    printf("[read_file_by_path]path:%s buf=%s\n",absolutePath,buf);
								    len=read_file_by_path(absolutePath,&buf);
								    printf("[read_file_by_path]ret:%d buf=%s\n",len,(char*)buf);
								    
								    for(int k=strlen(absolutePath)-1;k>=0;k--){
				                        if(absolutePath[k]=='/'&&k!=0){
					                        absolutePath[k]='\0';
					                
					                        printf("[create_file]pwd:%s fname=%s\n",target,&absolutePath[k+1]);
					                        int ret=create_file(target,&absolutePath[k+1]);
					                        printf("[create_file]ret:%d\n",ret);
					                
					                        break;
				                        }
				                        else if(absolutePath[k]=='/'&&k==0){//已經切到root目錄了
				    	                    absolutePath[k]='\0';
				    	            
				    	                    printf("[create_file]pwd:%s fname=%s\n",target,&absolutePath[k+1]);
				    	                    int ret=create_file(target,&absolutePath[k+1]);
				    	                    printf("[create_file]ret:%d\n",ret);
				    	            
					                        break;
					                   }
				
		    	                    }
						
								    char writePath[100];
								    strcpy(writePath,target);
								    strcat(writePath,"/");
								    strcat(writePath,&absolutePath[k+1]);
								    
								    printf("[write_file_by_path]path:%s buf=%s size=%d\n",writePath,(char*)buf,len);
									int ret=write_file_by_path(writePath,(char*)buf,len);
									printf("[write_file_by_path]ret:%d\n",ret);
								}
								
							}
							else if(cpTarget==2){//單一檔案複製
								if(strcmp(target,"")!=0){
								    int len;
								    void *buf=NULL;
								    printf("[read_file_by_path]path:%s buf=%s\n",absolutePath,buf);
								    len=read_file_by_path(absolutePath,&buf);
								    printf("[read_file_by_path]ret:%d buf=%s\n",len,(char*)buf);
								    
								    if(strcmp(tempName,"")==0){
										for(int k=strlen(absolutePath)-1;k>=0;k--){
				                            if(absolutePath[k]=='/'&&k!=0){
					                            absolutePath[k]='\0';
					                
					                            printf("[create_file]pwd:%s fname=%s\n",target,&absolutePath[k+1]);
					                            int ret=create_file(target,&absolutePath[k+1]);
					                            printf("[create_file]ret:%d\n",ret);
					                
					                            break;
				                            }
				                            else if(absolutePath[k]=='/'&&k==0){//已經切到root目錄了
				    	                        absolutePath[k]='\0';
				    	            
				    	                        printf("[create_file]pwd:%s fname=%s\n",target,&absolutePath[k+1]);
				    	                        int ret=create_file(target,&absolutePath[k+1]);
				    	                        printf("[create_file]ret:%d\n",ret);
				    	            
					                            break;
					                       }
				
		    	                       }
									}
								    else{
										printf("[create_file]pwd:%s fname=%s\n",target,tempName);
				    	                int ret=create_file(target,tempName);
				    	                printf("[create_file]ret:%d\n",ret);
									}
									
									char writePath[100];
								    strcpy(writePath,target);
								    strcat(writePath,"/");
								    strcat(writePath,tempName);
								    
								    printf("[write_file_by_path]path:%s buf=%s size=%d\n",writePath,(char*)buf,len);
									int ret=write_file_by_path(writePath,(char*)buf,len);
									printf("[write_file_by_path]ret:%d\n",ret);
								    
								}
							}
							
						}
						else if(dir_change(absolutePath)<0){//找不到
						//else if(1){
							strcat(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							strcat(tempReturn,"No such file or directory.\n");
						}
						else if(dir_change(absolutePath)==0){//找到
						//else if(1){
							
						}
					}
					
					if(i==argc-1){
						 if(strcmp(tempReturn,"")!=0)
						     printf("%s",tempReturn);
					 }

				}
			}
		}
		else if(strcmp(argv[0],"mv")==0){//rename
			if(argc<=2)
			    printf("wrong format\n");
			else{
			
				/*if(argv[1][0]=='\"'&&argv[1][strlen(argv[1])-1]=='\"'){//檔名如果是""括起來，去掉""的部分 
				    argv[1][strlen(argv[1])-1]='\0';
				    argv[1]=&argv[1][1];
			    } */ 
			    
			    for(int i=1;i<=argc-1;i++){
					if(strcmp(argv[i],">")==0&&i==1&&(i+1)<=argc-1){//ex:mv > test.txt
						relative_to_absolute_path(argv[i+1],absolutePath);
						strcpy(tempReturn,"wrong format\n");
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}
					}
					else if(strcmp(argv[2],">")==0){
						printf("wrong format\n");
						break;
					}
					else if(strcmp(argv[i],">")==0&&(i+1)<=argc-1){
						relative_to_absolute_path(argv[i+1],absolutePath);
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}

					}
					else if(strcmp(argv[i],">")==0&&i==argc-1){
						printf("write file error\n");
						break;
					}
					else if(strcmp(argv[i],">")!=0&&i==1){
						relative_to_absolute_path(argv[i],absolutePath);
						if(search_file(absolutePath)==-1){//找不到
						//if(0){
							strcat(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							strcat(tempReturn,"No such file or directory.\n");
							
						}    
				        else if(search_file(absolutePath)==0){//找到
				        //else if(1){
							if(argv[2][0]=='\"'&&argv[2][strlen(argv[2])-1]=='\"'){//檔名如果是""括起來，去掉""的部分 
				                argv[2][strlen(argv[2])-1]='\0';
				                argv[2]=&argv[2][1];
			                }
			                
			                for(int k=strlen(absolutePath)-1;k>=0;k--){
				                if(absolutePath[k]=='/'&&k!=0){
					                absolutePath[k]='\0';
					                
					                printf("[rename_file]pwd:%s f1=%s f2=%s\n",absolutePath,&absolutePath[k+1],argv[i+1]);
					                int ret=rename_file(absolutePath,&absolutePath[k+1],argv[i+1]);
					                printf("[rename_file]ret:%d\n",ret);
					                
					                break;
				                }
				                else if(absolutePath[k]=='/'&&k==0){//已經切到root目錄了
				    	            absolutePath[k]='\0';
				    	            
				    	            printf("[rename_file]pwd:%s f1=%s f2=%s\n","/",&absolutePath[k+1],argv[i+1]);
				    	            int ret=rename_file("/",&absolutePath[k+1],argv[i+1]);
				    	            printf("[rename_file]ret:%d\n",ret);
				    	            
					                break;
					            }
				
		    	             }
							
							
						}
						
					}
					
					if(i==argc-1){
						 if(strcmp(tempReturn,"")!=0)
						     printf("%s",tempReturn);
					 }

				}
				
			    
				
                
			} 
		}
		else if(strcmp(argv[0],"mvdir")==0){
			if(argc<=2)
			    printf("wrong format\n");
			else{
			
				/*if(argv[1][0]=='\"'&&argv[1][strlen(argv[1])-1]=='\"'){//檔名如果是""括起來，去掉""的部分 
				    argv[1][strlen(argv[1])-1]='\0';
				    argv[1]=&argv[1][1];
			    } */ 
			    
			    for(int i=1;i<=argc-1;i++){
					if(strcmp(argv[i],">")==0&&i==1&&(i+1)<=argc-1){//ex:mv > test.txt
						relative_to_absolute_path(argv[i+1],absolutePath);
						strcpy(tempReturn,"wrong format\n");
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}
					}
					else if(strcmp(argv[2],">")==0){
						printf("wrong format\n");
						break;
					}
					else if(strcmp(argv[i],">")==0&&(i+1)<=argc-1){
						relative_to_absolute_path(argv[i+1],absolutePath);
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}

					}
					else if(strcmp(argv[i],">")==0&&i==argc-1){
						printf("write file error\n");
						break;
					}
					else if(strcmp(argv[i],">")!=0&&i==1){
						relative_to_absolute_path(argv[i],absolutePath);
						if(dir_change(absolutePath)==-1){//找不到
						//if(0){
							strcat(tempReturn,argv[i]);
							strcat(tempReturn,":\n");
							strcat(tempReturn,"No such file or directory.\n");
							
						}    
				        else if(dir_change(absolutePath)==0){//找到
				        //else if(1){
							if(argv[2][0]=='\"'&&argv[2][strlen(argv[2])-1]=='\"'){//檔名如果是""括起來，去掉""的部分 
				                argv[2][strlen(argv[2])-1]='\0';
				                argv[2]=&argv[2][1];
			                }
			                
			                for(int k=strlen(absolutePath)-1;k>=0;k--){
				                if(absolutePath[k]=='/'&&k!=0){
					                absolutePath[k]='\0';
					                
					                printf("[dir_rename]pwd:%s f1=%s f2=%s\n",absolutePath,&absolutePath[k+1],argv[i+1]);
					                int ret=dir_rename(absolutePath,&absolutePath[k+1],argv[i+1]);
					                printf("[dir_rename]ret:%d\n",ret);
					                
					                break;
				                }
				                else if(absolutePath[k]=='/'&&k==0){//已經切到root目錄了
				    	            absolutePath[k]='\0';
				    	            
				    	            printf("[dir_rename]pwd:%s f1=%s f2=%s\n","/",&absolutePath[k+1],argv[i+1]);
				    	            int ret=dir_rename("/",&absolutePath[k+1],argv[i+1]);
				    	            printf("[dir_rename]ret:%d\n",ret);
					                break;
					            }
				
		    	             }
							
							
						}
						
					}
					
					if(i==argc-1){
						 if(strcmp(tempReturn,"")!=0)
						     printf("%s",tempReturn);
					 }

				}
				
			    
				
                
			} 
		}
		else if(strcmp(argv[0],"touch")==0){
			if(argc==1)
			    printf("wrong format\n");
            else{
				for(int i=1;i<=argc-1;i++){
					if(strcmp(argv[i],">")==0&&i==1&&(i+1)<=argc-1){//ex:touch > test.txt
						relative_to_absolute_path(argv[i+1],absolutePath);
						strcpy(tempReturn,"wrong format\n");
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}
					}
					else if(strcmp(argv[i],">")==0&&(i+1)<=argc-1){
						relative_to_absolute_path(argv[i+1],absolutePath);
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}

					}
					else if(strcmp(argv[i],">")==0&&i==argc-1){
						printf("write file error\n");
						break;
					}
					else if(strcmp(argv[i],">")!=0){
						relative_to_absolute_path(argv[i],absolutePath);
						if(search_file(absolutePath)==-1){//找不到就直接創建
						//if(0){
							for(int k=strlen(absolutePath)-1;k>=0;k--){
				                if(absolutePath[k]=='/'&&k!=0){
					                absolutePath[k]='\0';
					                
					                printf("[create_file]pwd:%s fname:%s\n",absolutePath,&absolutePath[k+1]);
					                int ret=create_file(absolutePath,&absolutePath[k+1]);
					                printf("[create_file]ret:%d\n",ret);
					                break;
				                }
				                else if(absolutePath[k]=='/'&&k==0){//已經切到root目錄了
				    	            absolutePath[k]='\0';
				    	            
				    	            printf("[create_file]pwd:%s fname:%s\n","/",&absolutePath[k+1]);
				    	            int ret=create_file("/",&absolutePath[k+1]);
				    	            printf("[create_file]ret:%d\n",ret);
					                break;
					            }
				
		    	             }
						}    
				        else{//找到同名就忽略
							
						}
					}
					
					if(i==argc-1){
						 if(strcmp(tempReturn,"")!=0)
						     printf("%s",tempReturn);
					 }

				}
			}
		}
		else if(strcmp(argv[0],"echo")==0){
			if(argc==1)
			    printf("wrong format\n");
            else{
				for(int i=1;i<=argc-1;i++){
					if(strcmp(argv[i],">")==0&&i==1&&(i+1)<=argc-1){//ex:echo > test.txt
						relative_to_absolute_path(argv[i+1],absolutePath);
						strcpy(tempReturn,"wrong format\n");
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}
					}
					else if(strcmp(argv[i],">")==0&&(i+1)<=argc-1){
						relative_to_absolute_path(argv[i+1],absolutePath);
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}

					}
					else if(strcmp(argv[i],">")==0&&i==argc-1){
						printf("write file error\n");
						break;
					}
					else if(strcmp(argv[i],">")!=0){
						
						if(argv[i][0]=='\"'&&argv[i][strlen(argv[i])-1]=='\"'){//檔名如果是""括起來，去掉""的部分 
				            argv[i][strlen(argv[i])-1]='\0';
				            argv[i]=&argv[i][1];
			            }  
			            
						if(i!=1)
						    strcat(tempReturn," ");
						strcat(tempReturn,argv[i]);
					}
					
					if(i==argc-1){
						 if(strcmp(tempReturn,"")!=0)
						     printf("%s\n",tempReturn);
					 }

				}
			}
		}
		else if(strcmp(argv[0],"mkdir")==0){
			if(argc==1)
			    printf("wrong format\n");
            else{
				for(int i=1;i<=argc-1;i++){
					if(strcmp(argv[i],">")==0&&i==1&&(i+1)<=argc-1){//ex:mkdir > test.txt
						relative_to_absolute_path(argv[i+1],absolutePath);
						strcpy(tempReturn,"wrong format\n");
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}
					}
					else if(strcmp(argv[i],">")==0&&(i+1)<=argc-1){
						relative_to_absolute_path(argv[i+1],absolutePath);
						
						printf("[write_file_by_path]path:%s buf=%s size=%d\n",absolutePath,tempReturn,strlen(tempReturn));
						
						if(write_file_by_path(absolutePath,tempReturn,strlen(tempReturn))>=0){
						//if(1){
							printf("%s finish\n",tempReturn);
							break;
						}   
						else{
							printf("write file error.\n");
							break;
						}

					}
					else if(strcmp(argv[i],">")==0&&i==argc-1){
						printf("write file error\n");
						break;
					}
					else if(strcmp(argv[i],">")!=0){
						relative_to_absolute_path(argv[i],absolutePath);
						if(dir_change(absolutePath)<0){//找不到就直接創建
						//if(0){
							for(int k=strlen(absolutePath)-1;k>=0;k--){
				                if(absolutePath[k]=='/'&&k!=0){
					                absolutePath[k]='\0';
					                
					                printf("[dir_create]pwd:%s fname:%s\n",absolutePath,&absolutePath[k+1]);
					                int ret=dir_create(absolutePath,&absolutePath[k+1]);
					                printf("[dir_create]ret:%d\n",ret);
					                break;
				                }
				                else if(absolutePath[k]=='/'&&k==0){//已經切到root目錄了
				    	            absolutePath[k]='\0';
				    	            
				    	            printf("[dir_create]pwd:%s fname:%s\n","/",&absolutePath[k+1]);
				    	            int ret=dir_create("/",&absolutePath[k+1]);
				    	            printf("[dir_create]ret:%d\n",ret);
					                break;
					            }
				
		    	             }
						}    
				        else{//找到同名就忽略
							
						}
					}
					
					if(i==argc-1){
						 if(strcmp(tempReturn,"")!=0)
						     printf("%s",tempReturn);
					 }

				}
			}
		}
		else if(strcmp(argv[0],"dumpe2fs")==0){
		}
		else if(strcmp(argv[0],"find")==0){
			if(argc==3){
			    relative_to_absolute_path(argv[1],absolutePath);
			    if(dir_search(absolutePath,argv[2])<0){//find no dir
					strcat(absolutePath,"/");
					strcat(absolutePath,argv[2]);
					if(search_file(absolutePath)<0){//find no file
					     printf("No such file or directory\n");
				    }
				    else{//find file
						printf("%s\n",argv[2]);
					}
				}
			}
			else
			    printf("wrong format\n");
			
		}
		else if(strcmp(argv[0],"help")==0){
			printf("**********Command List**********\n");
            printf("1.ls [Dir]...                \t list files under directories.\n");
            printf("2.cd [Dir]                   \t change to [Dir].\n");
            printf("3.cp [file]...[Dir]          \t copy files to [Dir].\n");
            printf("4.rm [file]...               \t remove files.\n");
            printf("5.rmdir [Dir]...             \t remove directories.\n");
            printf("6.touch [file]...            \t create files.\n");
            printf("7.mkdir [Dir]...             \t create directories.\n");
            printf("8.mv [file] [filename]       \t rename the file.\n");
            printf("9.mvdir [Dir] [directoryname]\t rename the directory.\n");
            printf("10.cat [file]...             \t read files.\n");
            printf("11.pwd                       \t print the current directory path.\n");
            printf("12.echo [content] > [file]   \t write content to the file.\n");
            printf("13.find [Dir] [file/Dir]     \t find specific file or directory\n");
		}
		else
		    printf("wrong commmand\n");
		
		
                       
	}
	
	return 0;
}
