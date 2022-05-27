#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

//判断当前字符串是否为关键字
bool isKey(string s){
    //关键字数组
    string keyArray[21] = {"int","char","string","bool","float","double","float","true","false","return",
                        "if","else","while","for","default","case","do","public","static","switch"};
    //与当前字符串一一对比
    for(int i=0;i<21;i++){
        if(keyArray[i] == s){
            return true;
        }
    }
    return false;
}

//判断当前字符是否是运算符
bool isOperator(char ch){
    if('+'==ch || '-'==ch || '*'==ch || '/'==ch || '%'==ch || '^'==ch || '='==ch || '<'==ch || '>'==ch || '!'==ch)
        return true;
    else
        return false;
}

//判断当前字符是否是分隔符
bool isSeparator(char ch){
    if('"'==ch || ':'==ch ||','==ch || ';'==ch || '{'==ch || '}'==ch || '('==ch || ')'==ch)
        return true;
    else
        return false;
}


int main( )
{
    //定义字符变量，保存从源程序中读取的单个字符
    char ch;
    //定义字符串，保存从源程序中连续读取的字符串
    string result;
    //存放每个获取的单词的值
    string resultArray[999];
    //记录获取单词的个数
    int resultNum=0;

    string file;
	cout << "输入待词法分析的代码文件名？（eg. test1.txt）" << endl;
	cin >> file;
    ifstream infile;
    //将文件流对象与文件连接起来
    infile.open(file.data());
    //若失败,则输出错误消息,并终止程序运行
    assert(infile.is_open());
    //txt文本中读取空格符与换行符
    //infile >> noskipws;
    //txt文本中不读取空格符与换行符
    //infile >> skipws;
    //读取文本中的一个字符
    infile>>ch;

    while (!infile.eof())
    {
        //ch是英文字母或者下划线
        if(isalpha(ch) || '_'==ch){
            result.append(1,ch);
            infile>>ch;
            //判断是否为关键字
            while(1){
            	if(isKey(result)){
	                resultArray[resultNum++]="(基本保留字,\""+result+"\")";
	                result="";
	                break;
	            }else{
	            	//读入首字符为字母，继续读入字母、数字、下划线，组成标识符或者关键字
	            	if(isalpha(ch) || isdigit(ch) || '_'==ch){
		                result.append(1,ch);
		                infile>>ch;
		                if(isKey(result)){
		                    resultArray[resultNum++]="(基本保留字,\""+result+"\")";
		                    result="";
		                    break;
		                }
		            }else{
		            	//读入操作符或者运算符，正确保存标识符或者关键字
			            if(isSeparator(ch) || isOperator(ch)){
			                if(isKey(result)){
			                    resultArray[resultNum++]="(基本保留字,\""+result+"\")";
			                    result="";
			                    break;
			                }
			                else{
			                    if(isdigit(result.at(0))){
			                        resultArray[resultNum++]="(Error,\""+result+"\")";
			                    }else{
			                        resultArray[resultNum++]="(标识符,\""+result+"\")";
			                    }
			                    result="";
			                    break;
			                }
			            }
			            //读入不是字母、数字、运算符、标识符，继续读入直到遇到运算符或者分隔符
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
        //读入数字
        else if(isdigit(ch)){
        	int flag=0; 
            result.append(1,ch);
            infile>>ch;
            //继续读入数字，组成常数
            while(isdigit(ch)){
                result.append(1,ch);
                infile>>ch;
            }
            //遇到小数点.
			if(ch=='.'){
				result.append(1,ch);
				infile>>ch;
				flag=1;
			} 
			//继续读入数字，组成小数
            while(isdigit(ch)){
                result.append(1,ch);
                infile>>ch;
            }
            //遇到操作符或者运算符，正常终止
            if(isOperator(ch) || isSeparator(ch)){
                if(!flag) resultArray[resultNum++]="(常数,\""+result+"\")";
                else resultArray[resultNum++]="(浮点数,\""+result+"\")";
                result="";
                continue;
            }
            //读入其他错误字符
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
        //遇到运算符
        else if(isOperator(ch)){
            result.append(1,ch);
            infile>>ch;
            //判断是否存在//
            int flag = 0;
			if("/"==result){
				if('/'==ch){
					//txt文本中读取空格符与换行符
    				infile >> noskipws;
    				while(1){
    					infile>>ch;
    					if(ch == '\n'){
    						result="";
    						//txt文本中不读取空格符与换行符
    						infile >> skipws;
    						infile>>ch;
    						flag = 1;
    						break;
						}
					}
				}
			} 
			if(flag==1)continue; 
            //判断是否存在<=、>=、!=
            if("<"==result || ">"==result || "!"==result||"="==result){
                if('='==ch){
                    result.append(1,ch);
                    infile>>ch;
                }
            }
            //下一个读入符为字母、数字、分隔符，即正确
            if(isalpha(ch) || isdigit(ch) || isSeparator(ch)){
                resultArray[resultNum++]="(运算符,\""+result+"\")";
                result="";
                continue;
            }else if(isOperator(ch)){
            	continue;
            }else{
                //将错误输入符一起读入，直到正确
                while(!isSeparator(ch) && !isalpha(ch) && !isdigit(ch)){
                    result.append(1,ch);
                    infile>>ch;
                }
                resultArray[resultNum++]="(Error,\""+result+"\")";
                result="";
                continue;
            }
        }
        //读取到分隔符
        else if(isSeparator(ch)){
            result.append(1,ch);
            resultArray[resultNum++]="(分隔符,\""+result+"\")";
            result="";
            infile>>ch;
        }
        //读取到未定义输入
        else{
            //出错处理
            result.append(1,ch);
            resultArray[resultNum++]="(Error,\""+result+"\")";
            result="";
            infile>>ch;

        }
    }
    //关闭文件输入流
    infile.close();
    //以 （单词类编码，值） 输出结果
    for(int i=0;i<resultNum;i++) cout<< resultArray[i] <<endl;
  	return 0;
}
