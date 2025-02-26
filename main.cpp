#include <iostream>
#include <fstream>
#include "img.h"
#include <opencv2/opencv.hpp>

int main() {
    while(1){
        cout<<"==================================="<<endl;
        cout<<">>1、读取并展示图片"<<endl;
        cout<<">>2、压缩或解压图片"<<endl;
        cout<<">>3、图片转灰度图"<<endl;
        cout<<">>4、缩放图片"<<endl;
        cout<<"==================================="<<endl;
        cin.sync();
        cin.clear();
        cout<<"输入操作码： ";
        int opcode;
        cin>>opcode;
        if(cin.fail()){
            cin.sync();
            cin.clear();
            continue;
        }

        switch(opcode){
            case 1:{
                cout<<"输入文件名： ";
                string name;
                cin>>name;
                Picture pic;
                pic.read_img(name);
                break;
            }
            case 2:{
                bool flag=true;
                while(flag){
                    cout<<">>1、压缩"<<endl;
                    cout<<">>2、解压"<<endl;
                    cout<<"请选择操作(输入\"0\"退出)： ";
                    int choose;
                    cin.sync();
                    cin.clear();
                    cin>>choose;
                    if(cin.fail()){
                        cout<<"ERROR"<<endl;
                        break;
                    }
                    switch(choose){
                        case 0:{
                            flag=false;
                            break;
                        }
                        case 1:{
                            cout<<"输入文件名： ";
                            string name;
                            cin>>name;
                            Picture pic;
                            if(pic.check(name,0)){
                                pic.getCount(name);
                                pic.BuildHuffmanTree();
                                pic.getCodeTable();
                                pic.compress(name);
                            }
                            else{
                                cout<<"文件名字错误"<<endl;
                            }
                            break;
                        }
                        case 2:{
                            cout<<"输入文件名： ";
                            string name;
                            cin>>name;
                            Picture pic;
                            if(pic.check(name,1))
                                pic.decompress(name);
                            else
                                cout<<"文件名字错误"<<endl;
                            break;
                        }
                        default:{
                            break;
                        }
                    }
                }
                break;
            }
            case 3:{
                cout<<"输入文件名： ";
                string name;
                cin>>name;
                Picture pic;
                pic.gray_img(name);
                break;
            }
            case 4:{
                cout<<"输入文件名： ";
                string name;
                cin>>name;
                Picture pic;
                pic.resize_img(name);
                break;
            }
            default:{
                break;
            }
        }
    }
    system("Pause");
}