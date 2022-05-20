#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

//�жϵ�ǰ�ַ����Ƿ�Ϊ�ؼ���
bool isKey(string s){
    //�ؼ�������
    string keyArray[21] = {"int","char","string","bool","float","double","float","true","false","return",
                        "if","else","while","for","default","case","do","public","static","switch"};
    //�뵱ǰ�ַ���һһ�Ա�
    for(int i=0;i<21;i++){
        if(keyArray[i] == s){
            return true;
        }
    }
    return false;
}

//�жϵ�ǰ�ַ��Ƿ��������
bool isOperator(char ch){
    if('+'==ch || '-'==ch || '*'==ch || '/'==ch || '%'==ch || '^'==ch || '='==ch || '<'==ch || '>'==ch || '!'==ch)
        return true;
    else
        return false;
}

//�жϵ�ǰ�ַ��Ƿ��Ƿָ���
bool isSeparator(char ch){
    if('"'==ch || ':'==ch ||','==ch || ';'==ch || '{'==ch || '}'==ch || '('==ch || ')'==ch)
        return true;
    else
        return false;
}


int main( )
{
    //�����ַ������������Դ�����ж�ȡ�ĵ����ַ�
    char ch;
    //�����ַ����������Դ������������ȡ���ַ���
    string result;
    //���ÿ����ȡ�ĵ��ʵ�ֵ
    string resultArray[999];
    //��¼��ȡ���ʵĸ���
    int resultNum=0;

    string file;
	cout << "������ʷ������Ĵ����ļ�������eg. test1.txt��" << endl;
	cin >> file;
    ifstream infile;
    //���ļ����������ļ���������
    infile.open(file.data());
    //��ʧ��,�����������Ϣ,����ֹ��������
    assert(infile.is_open());
    //txt�ı��ж�ȡ�ո���뻻�з�
    //infile >> noskipws;
    //txt�ı��в���ȡ�ո���뻻�з�
    //infile >> skipws;
    //��ȡ�ı��е�һ���ַ�
    infile>>ch;

    while (!infile.eof())
    {
        //ch��Ӣ����ĸ�����»���
        if(isalpha(ch) || '_'==ch){
            result.append(1,ch);
            infile>>ch;
            //�ж��Ƿ�Ϊ�ؼ���
            while(1){
            	if(isKey(result)){
	                resultArray[resultNum++]="(����������,\""+result+"\")";
	                result="";
	                break;
	            }else{
	            	//�������ַ�Ϊ��ĸ������������ĸ�����֡��»��ߣ���ɱ�ʶ�����߹ؼ���
	            	if(isalpha(ch) || isdigit(ch) || '_'==ch){
		                result.append(1,ch);
		                infile>>ch;
		                if(isKey(result)){
		                    resultArray[resultNum++]="(����������,\""+result+"\")";
		                    result="";
		                    break;
		                }
		            }else{
		            	//����������������������ȷ�����ʶ�����߹ؼ���
			            if(isSeparator(ch) || isOperator(ch)){
			                if(isKey(result)){
			                    resultArray[resultNum++]="(����������,\""+result+"\")";
			                    result="";
			                    break;
			                }
			                else{
			                    if(isdigit(result.at(0))){
			                        resultArray[resultNum++]="(Error,\""+result+"\")";
			                    }else{
			                        resultArray[resultNum++]="(��ʶ��,\""+result+"\")";
			                    }
			                    result="";
			                    break;
			                }
			            }
			            //���벻����ĸ�����֡����������ʶ������������ֱ��������������߷ָ���
			            else{
			                result.append(1,ch);
			                infile>>ch;
			                while(!isSeparator(ch) && !isOperator(ch)){
			                    result.append(1,ch);
			                    infile>>ch;
			                }
			                resultArray[resultNum++]="(Error,\""+result+"\")";
			                result="";
			                continue;
			            }
					}
				}
			}
        }
        //��������
        else if(isdigit(ch)){
        	int flag=0; 
            result.append(1,ch);
            infile>>ch;
            //�����������֣���ɳ���
            while(isdigit(ch)){
                result.append(1,ch);
                infile>>ch;
            }
            //����С����.
			if(ch=='.'){
				result.append(1,ch);
				infile>>ch;
				flag=1;
			} 
			//�����������֣����С��
            while(isdigit(ch)){
                result.append(1,ch);
                infile>>ch;
            }
            //���������������������������ֹ
            if(isOperator(ch) || isSeparator(ch)){
                if(!flag) resultArray[resultNum++]="(����,\""+result+"\")";
                else resultArray[resultNum++]="(������,\""+result+"\")";
                result="";
                continue;
            }
            //�������������ַ�
            else{
                result.append(1,ch);
                infile>>ch;
                while(!isSeparator(ch) && !isOperator(ch)){
                    result.append(1,ch);
                    infile>>ch;
                }
                resultArray[resultNum++]="(Error,\""+result+"\")";
                result="";
                continue;
            }
        }
        //���������
        else if(isOperator(ch)){
            result.append(1,ch);
            infile>>ch;
            //�ж��Ƿ����//
            int flag = 0;
			if("/"==result){
				if('/'==ch){
					//txt�ı��ж�ȡ�ո���뻻�з�
    				infile >> noskipws;
    				while(1){
    					infile>>ch;
    					if(ch == '\n'){
    						result="";
    						//txt�ı��в���ȡ�ո���뻻�з�
    						infile >> skipws;
    						infile>>ch;
    						flag = 1;
    						break;
						}
					}
				}
			} 
			if(flag==1)continue; 
            //�ж��Ƿ����<=��>=��!=
            if("<"==result || ">"==result || "!"==result||"="==result){
                if('='==ch){
                    result.append(1,ch);
                    infile>>ch;
                }
            }
            //��һ�������Ϊ��ĸ�����֡��ָ���������ȷ
            if(isalpha(ch) || isdigit(ch) || isSeparator(ch)){
                resultArray[resultNum++]="(�����,\""+result+"\")";
                result="";
                continue;
            }else if(isOperator(ch)){
            	continue;
            }else{
                //�����������һ����룬ֱ����ȷ
                while(!isSeparator(ch) && !isalpha(ch) && !isdigit(ch)){
                    result.append(1,ch);
                    infile>>ch;
                }
                resultArray[resultNum++]="(Error,\""+result+"\")";
                result="";
                continue;
            }
        }
        //��ȡ���ָ���
        else if(isSeparator(ch)){
            result.append(1,ch);
            resultArray[resultNum++]="(�ָ���,\""+result+"\")";
            result="";
            infile>>ch;
        }
        //��ȡ��δ��������
        else{
            //������
            result.append(1,ch);
            resultArray[resultNum++]="(Error,\""+result+"\")";
            result="";
            infile>>ch;

        }
    }
    //�ر��ļ�������
    infile.close();
    //�� ����������룬ֵ�� ������
    for(int i=0;i<resultNum;i++) cout<< resultArray[i] <<endl;
  	return 0;
}
