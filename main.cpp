#include <iostream>
#include <fstream>
#include "img.h"
#include <opencv2/opencv.hpp>

int main() {
    while(1){
        cout<<"==================================="<<endl;
        cout<<">>1����ȡ��չʾͼƬ"<<endl;
        cout<<">>2��ѹ�����ѹͼƬ"<<endl;
        cout<<">>3��ͼƬת�Ҷ�ͼ"<<endl;
        cout<<">>4������ͼƬ"<<endl;
        cout<<"==================================="<<endl;
        cin.sync();
        cin.clear();
        cout<<"��������룺 ";
        int opcode;
        cin>>opcode;
        if(cin.fail()){
            cin.sync();
            cin.clear();
            continue;
        }

        switch(opcode){
            case 1:{
                cout<<"�����ļ����� ";
                string name;
                cin>>name;
                Picture pic;
                pic.read_img(name);
                break;
            }
            case 2:{
                bool flag=true;
                while(flag){
                    cout<<">>1��ѹ��"<<endl;
                    cout<<">>2����ѹ"<<endl;
                    cout<<"��ѡ�����(����\"0\"�˳�)�� ";
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
                            cout<<"�����ļ����� ";
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
                                cout<<"�ļ����ִ���"<<endl;
                            }
                            break;
                        }
                        case 2:{
                            cout<<"�����ļ����� ";
                            string name;
                            cin>>name;
                            Picture pic;
                            if(pic.check(name,1))
                                pic.decompress(name);
                            else
                                cout<<"�ļ����ִ���"<<endl;
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
                cout<<"�����ļ����� ";
                string name;
                cin>>name;
                Picture pic;
                pic.gray_img(name);
                break;
            }
            case 4:{
                cout<<"�����ļ����� ";
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