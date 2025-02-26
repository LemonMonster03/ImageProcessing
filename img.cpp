#include "img.h"
#include <iostream>
#include <fstream>

void Picture::delnode(TreeNode* &node){
	if(node==nullptr)
		return;
	delnode(node->left);
	delnode(node->right);
	delete node;
	node=nullptr;
}

void Picture::read_img(FILE_NAME name){
    ifstream ppm_file(name + ".ppm");

    if (!ppm_file.is_open())
    {
        cout<<"�ļ���ʧ��,�����ļ������Ƿ���ȷ"<<endl;
        return;
    }
    string format;
    ppm_file >> format;

    if(format=="P3"){
        int width,height,maxVal;
        ppm_file>>width>>height>>maxVal;

        cv::Mat img(height,width,CV_8UC3);

        for (int r=0;r<height;r+=1) {
            for (int c=0;c<width;c+=1) {
                int R,G,B;
                ppm_file>>R>>G>>B;
                img.at<cv::Vec3b>(r,c)=cv::Vec3b(B,G,R);
            }
        }

        ppm_file.close();
        imshow("Picture",img);
        waitKey(0);
    }
    else if(format=="P2"){
        int width,height,maxVal;
        ppm_file>>width>>height>>maxVal;

        cv::Mat img(height,width,CV_8UC1);

        for (int r=0;r<height;r+=1) {
            for (int c=0;c<width;c+=1) {
                int gray;
                ppm_file>>gray;
                img.at<uchar>(r,c)=static_cast<uchar>(gray);
            }
        }

        ppm_file.close();
        imshow("Picture",img);
        waitKey(0);
    }
    else{
        cout<<"�ݲ�֧�ֵĸ�ʽ"<<endl;
        return;
    }

    // for(int i=0;i<width*height;i++){
    //     ppm_file >> pixel_matrix.data[i].val.R >> pixel_matrix.data[i].val.G >> pixel_matrix.data[i].val.B;
    //     pixel_matrix.data[i].rol = i / width;
    //     pixel_matrix.data[i].col = i % width;
    // }
    // ppm_file.close();

    // cout<<(*CharCount.begin()).first.R;

    // for(int i=0;i<pixel_matrix.size;i++){
    //     cout<<"rol:"<<pixel_matrix.data[i].rol<<" and col:"<<pixel_matrix.data[i].col<<" is:"<<pixel_matrix.data[i].val.R<<" "<<pixel_matrix.data[i].val.G<<" "<<pixel_matrix.data[i].val.B<<endl;
    // }

    // ofstream hfm_file(name+".hfm");
    // hfm_file<<width<<" "<<height<<" "<<max_value<<" ";

    // for(int i=0;i<pixel_matrix.size;i++){
    //     hfm_file<<pixel_matrix.data[i].val.R<<" "<<pixel_matrix.data[i].val.G<<" "<<pixel_matrix.data[i].val.B<<" ";
    // }
    
}

void Picture::gray_img(FILE_NAME name){
    ifstream ppm_file(name + ".ppm");

    if (!ppm_file.is_open())
    {
        cout<<"�ļ���ʧ��,�����ļ������Ƿ���ȷ"<<endl;
        return;
    }
    string format;
    ppm_file >> format;

    if(format=="P3"){
        int width,height,maxVal;
        ppm_file>>width>>height>>maxVal;

        cv::Mat rgb_img(height,width,CV_8UC3);
        cv::Mat gray_img(height,width,CV_8UC1);

        for (int r=0;r<height;r+=1) {
            for (int c=0;c<width;c+=1) {
                int R,G,B;
                ppm_file>>R>>G>>B;
                int gray=(R*299+G*587+B*114)/1000;
                gray_img.at<uchar>(r,c)=static_cast<uchar>(gray);
            }
        }

        ppm_file.close();
        const string temp=name+="_gray.jpg";
        imwrite(temp,gray_img);
        cout<<"ͼƬ�ѱ���Ϊ "<<temp<<endl;
        imshow("Picture_Gray",gray_img);
        waitKey(0);
    }
    else if(format=="P2"){
        cout<<"ͼƬ�Ѿ��ǻҶ�ͼ"<<endl;
        return;
    }
    else{
        cout<<"�ݲ�֧�ֵĸ�ʽ"<<endl;
        return;
    }
}

void Picture::resize_img(FILE_NAME name){
    ifstream ppm_file(name + ".ppm");

    if (!ppm_file.is_open())
    {
        cout<<"�ļ���ʧ��,�����ļ������Ƿ���ȷ"<<endl;
        return;
    }
    string format;
    ppm_file >> format;
    int w,h;
    cout<<"���������ź�ĸ߶ȣ�";
    cin>>h;
    cout<<"���������ź�ĸ߶ȣ�";
    cin>>w;

    if(format=="P3"){
        int width,height,maxVal;
        ppm_file>>width>>height>>maxVal;

        cv::Mat ori_img(height,width,CV_8UC3);
        for (int r=0;r<height;r+=1) {
            for (int c=0;c<width;c+=1) {
                int R,G,B;
                ppm_file>>R>>G>>B;
                ori_img.at<cv::Vec3b>(r,c)=cv::Vec3b(B,G,R);
            }
        }

        cv::Mat des_img(h,w,CV_8UC3);

        for(int r=0;r<h;r++){
            for(int c=0;c<w;c++){
                double ori_y=((double)r+0.5)*(double)height/(double)h-0.5;
                double ori_x=((double)c+0.5)*(double)width/(double)w-0.5;

                int left_x=static_cast<int>(ori_x);
                int right_x=left_x+1;
                int down_y=static_cast<int>(ori_y);
                int up_y=down_y+1;

                if((down_y>=height-1)&&(left_x>=width-1)){
                    for(int i=0;i<3;i++){
                        des_img.at<Vec3b>(r,c)[i]=((double)right_x-ori_x)*((double)up_y-ori_y)*ori_img.at<Vec3b>(down_y,left_x)[i];
                    }
                }
                else if(down_y>=height-1){
                    for(int i=0;i<3;i++){
                        des_img.at<Vec3b>(r,c)[i]=((double)right_x-ori_x)*((double)up_y-ori_y)*ori_img.at<Vec3b>(down_y,left_x)[i]
                        +(ori_x-(double)left_x)*((double)up_y-ori_y)*ori_img.at<Vec3b>(down_y,right_x)[i];
                    }
                }
                else if(left_x>=width-1){
                     for(int i=0;i<3;i++){
                        des_img.at<Vec3b>(r,c)[i]=((double)right_x-ori_x)*((double)up_y-ori_y)*ori_img.at<Vec3b>(down_y,left_x)[i]
                        +((double)right_x-ori_x)*(ori_y-(double)down_y)*ori_img.at<Vec3b>(up_y,left_x)[i];
                    }                   
                }
                else{
                    for(int i=0;i<3;i++){
                        des_img.at<Vec3b>(r,c)[i]=((double)right_x-ori_x)*((double)up_y-ori_y)*ori_img.at<Vec3b>(down_y,left_x)[i]
                        +(ori_x-(double)left_x)*((double)up_y-ori_y)*ori_img.at<Vec3b>(down_y,right_x)[i]
                        +((double)right_x-ori_x)*(ori_y-(double)down_y)*ori_img.at<Vec3b>(up_y,left_x)[i]
                        +(ori_x-(double)left_x)*(ori_y-(double)down_y)*ori_img.at<Vec3b>(up_y,right_x)[i];
                    }
                }
            }
        }
        
        ppm_file.close();
        const string temp=name+="_resize.jpg";
        cout<<"ͼƬ�ѱ���Ϊ "<<temp<<endl;
        imwrite(temp,des_img);
        imshow("Picture_resize",des_img);
        waitKey(0);
    }
    else if(format=="P2"){
        int width,height,maxVal;
        ppm_file>>width>>height>>maxVal;

        cv::Mat ori_img(height,width,CV_8UC1);
        for (int r=0;r<height;r+=1) {
            for (int c=0;c<width;c+=1) {
                int gray;
                ppm_file>>gray;
                ori_img.at<uchar>(r,c)=static_cast<uchar>(gray);
            }
        }

        cv::Mat des_img(h,w,CV_8UC1);
        for(int r=0;r<h;r++){
            for(int c=0;c<w;c++){
                double ori_y=((double)r+0.5)*(double)height/(double)h-0.5;
                double ori_x=((double)c+0.5)*(double)width/(double)w-0.5;

                int left_x=static_cast<int>(ori_x);
                int right_x=left_x+1;
                int down_y=static_cast<int>(ori_y);
                int up_y=down_y+1;

                if((down_y>=height-1)&&(left_x>=width-1)){
                    for(int i=0;i<1;i++){
                        des_img.at<uchar>(r,c)=static_cast<uchar>(((double)right_x-ori_x)*((double)up_y-ori_y)*ori_img.at<uchar>(down_y,left_x));
                    }
                }
                else if(down_y>=height-1){
                    for(int i=0;i<1;i++){
                        des_img.at<uchar>(r,c)=static_cast<uchar>(((double)right_x-ori_x)*((double)up_y-ori_y)*ori_img.at<uchar>(down_y,left_x)
                        +(ori_x-(double)left_x)*((double)up_y-ori_y)*ori_img.at<uchar>(down_y,right_x));
                    }
                }
                else if(left_x>=width-1){
                     for(int i=0;i<1;i++){
                        des_img.at<uchar>(r,c)=static_cast<uchar>(((double)right_x-ori_x)*((double)up_y-ori_y)*ori_img.at<uchar>(down_y,left_x)
                        +((double)right_x-ori_x)*(ori_y-(double)down_y)*ori_img.at<uchar>(up_y,left_x));
                    }                   
                }
                else{
                    for(int i=0;i<1;i++){
                        des_img.at<uchar>(r,c)=static_cast<uchar>(((double)right_x-ori_x)*((double)up_y-ori_y)*ori_img.at<uchar>(down_y,left_x)
                        +(ori_x-(double)left_x)*((double)up_y-ori_y)*ori_img.at<uchar>(down_y,right_x)
                        +((double)right_x-ori_x)*(ori_y-(double)down_y)*ori_img.at<uchar>(up_y,left_x)
                        +(ori_x-(double)left_x)*(ori_y-(double)down_y)*ori_img.at<uchar>(up_y,right_x));
                    }
                }
            }
        }
        
        ppm_file.close();
        const string temp=name+="_resize.jpg";
        cout<<"ͼƬ�ѱ���Ϊ "<<temp<<endl;
        imwrite(temp,des_img);
        imshow("Picture_resize",des_img);
        waitKey(0);
    }
    else{
        cout<<"�ݲ�֧�ֵĸ�ʽ"<<endl;
        return;
    }
}



bool Picture::getCount(FILE_NAME name){
    ifstream ppm_file(name + ".ppm");

    if (!ppm_file.is_open())
    {
        cout<<"�ļ���ʧ��,�����ļ������Ƿ���ȷ"<<endl;
        return false;
    }
    string format;
    ppm_file >> format;

    if(format=="P3"){
        int width,height,maxVal;
        ppm_file>>width>>height>>maxVal;
        for (int r=0;r<height;r+=1) {
            for (int c=0;c<width;c+=1) {
                int R,G,B;
                ppm_file>>R>>G>>B;
                RGB temp(R,G,B);
                rgb.push_back(temp);
                // cout<<"now push struct:<"<<temp.R<<" "<<temp.G<<" "<<temp.B<<">"<<endl;
            }
        }
        for(auto cur:rgb){
            FrequencyMap[cur]++;
        }
        count=FrequencyMap.size();
        // for(auto it:FrequencyMap){
        // std::cout << "Frequency of color: " <<(it.first).R<<" "<<(it.first).G<<" "<<(it.first).B<<" -> "<<it.second<< std::endl;	
	    // }
    }
    else if(format=="P2"){
        int width,height,maxVal;
        ppm_file>>width>>height>>maxVal;

        for (int r=0;r<height;r+=1) {
            for (int c=0;c<width;c+=1) {
                int gray;
                ppm_file>>gray;
                RGB temp(gray,0,0);
                rgb.push_back(temp);
            }
        }
        for(auto cur:rgb){
            FrequencyMap[cur]++;
        }
        count=FrequencyMap.size();
    }
    else{
        cout<<"�ݲ�֧�ֵĸ�ʽ"<<endl;
        return false;
    }
    return true;
}

void Picture::BuildHuffmanTree(){
	/*Ϊ�����ַ������ڵ㣬����ȫ�����*/
    priority_queue<TreeNode*, vector<TreeNode*>, cmpchar> node_queue;
    for(auto it=FrequencyMap.begin();it!=FrequencyMap.end();it++){
            node_queue.push(new TreeNode((*it).second,(*it).first,nullptr,nullptr));
            // cout<<(*it).first.R<<" "<<(*it).first.G<<" "<<(*it).first.B<<":"<<(*it).second<<endl;
    }
    
    while(node_queue.size()>1){
        TreeNode* temp1=node_queue.top();
        node_queue.pop();
        // cout<<"temp1:"<<temp1->data.R<<":"<<temp1->pro<<endl;
        TreeNode* temp2=node_queue.top();
        node_queue.pop();
        // cout<<"temp2:"<<temp2->data.R<<":"<<temp2->pro<<endl;
        TreeNode* newnode = new TreeNode(temp1->pro + temp2->pro,RGB(0,0,0),temp1,temp2);
        node_queue.push(newnode);
    }

    root=node_queue.top();
}

void Picture::getCodeTable(){
	dfs(root,"");

    // for(auto it:CodeTable){
    //     cout<<CodeTable.size()<<endl;
    //     cout<<it.first.R<<" "<<it.first.G<<" "<<it.first.B<<"��Ӧ�ı��룺"<<it.second<<endl;
    // }
}

void Picture::dfs(TreeNode* root,string code){
	if(!root)
		return;
	if((!root->left)&&(!root->right)){
		CodeTable[root->data]=code;
		// cout<<"����"<<root->data.R<<" "<<root->data.G<<" "<<root->data.B<<"�ı��룺"<<code<<endl;
		return;
	}
	dfs(root->left,code+"0");
	dfs(root->right,code+"1");
}

void Picture::compress(FILE_NAME name){
    ifstream inFile(name+".ppm");
    if (!inFile.is_open())
    {
        cout<<"�ļ���ʧ��,�����ļ������Ƿ���ȷ"<<endl;
        return;
    }
    ofstream outFile(name+".hfm",ios::binary);
    if(!outFile.is_open()){
        cout<<"�����ļ��Ѿ����ڻ���Ȩ�޲����޷�����"<<endl;
        return;
    }


	inFile.seekg(0,ios::beg);
	string code="";
	unsigned char bitdata=0;
	int addition=0;

    string type;
    int width,height,max_color;
    inFile>>type>>width>>height>>max_color;
    // cout<<type<<width<<height<<max_color;
    if(type=="P3"){
        int r,g,b;
        while(!inFile.eof())
        {
            inFile>>r>>g>>b;
            RGB temp(r,g,b);
            code+=CodeTable[temp];
        }
        // cout<<"���ı�ѹ����ı��룺"<<code<<endl;
        
        addition=code.length()%8 ? (8-code.length()%8) : 0;//���ӵ�0�ĸ��� 
    //	cout<<"Ӧ�ø��ӵ�0�ĸ�����"<<addition<<endl; 
        
        for(int i=0;i<addition;i++){
            code+="0";
        }
        // cout<<"���ı�ѹ����ı��룺"<<code<<endl;

        /*д�븽��0����*/
        outFile.write(reinterpret_cast<char*>(&addition), sizeof(int));

    //	cout<<"�ַ��ĸ�����"<<count<<endl; 
        /*д��RGBԪ�ظ���*/
        outFile.write(reinterpret_cast<char*>(&count), sizeof(int));

        outFile.write(type.c_str(), type.length());//����
        outFile.write(reinterpret_cast<char*>(&width),sizeof(int));//���
        outFile.write(reinterpret_cast<char*>(&height),sizeof(int));//�߶�
        outFile.write(reinterpret_cast<char*>(&max_color),sizeof(int));//���ֵ
        
        /*��RGB�Լ���ӦƵ��д���ļ�*/
        for(auto it=FrequencyMap.rbegin();it!=FrequencyMap.rend();it++){
            int a,b,c;
            RGB temp=(*it).first;
            a=temp.R;
            b=temp.G;
            c=temp.B;
            outFile.write(reinterpret_cast<const char*>(&a),sizeof(int));
            outFile.write(reinterpret_cast<const char*>(&b),sizeof(int));
            outFile.write(reinterpret_cast<const char*>(&c),sizeof(int));
            outFile.write(reinterpret_cast<char*>(&(*it).second),sizeof(int));
        }
        
        int len=code.length();
        char c=0;
        for(int i=0;i<len;i++){ 
            c <<= 1;
            if(code[i]=='1')
                c |= 1;
            if((i+1)%8==0){
                outFile.write(reinterpret_cast<char*>(&c),sizeof(char));
            }
        }
    }
    else if(type=="P2"){
        int gray;
        while(!inFile.eof())
        {
            inFile>>gray;
            RGB temp(gray,0,0);
            code+=CodeTable[temp];
        }

        // cout<<"���ı�ѹ����ı��룺"<<code<<endl;
        
        addition=code.length()%8 ? (8-code.length()%8) : 0;//���ӵ�0�ĸ��� 
    //	cout<<"Ӧ�ø��ӵ�0�ĸ�����"<<addition<<endl; 
        
        for(int i=0;i<addition;i++){
            code+="0";
        }
        // cout<<"���ı�ѹ����ı��룺"<<code<<endl;

        /*д�븽��0����*/
        outFile.write(reinterpret_cast<char*>(&addition), sizeof(int));

    //	cout<<"�ַ��ĸ�����"<<count<<endl; 
        /*д��RGBԪ�ظ���*/
        outFile.write(reinterpret_cast<char*>(&count), sizeof(int));

        outFile.write(type.c_str(), type.length());//����
        outFile.write(reinterpret_cast<char*>(&width),sizeof(int));//���
        outFile.write(reinterpret_cast<char*>(&height),sizeof(int));//�߶�
        outFile.write(reinterpret_cast<char*>(&max_color),sizeof(int));//���ֵ
        
        /*��RGB�Լ���ӦƵ��д���ļ�*/
        for(auto it=FrequencyMap.rbegin();it!=FrequencyMap.rend();it++){
            int a,b,c;
            RGB temp=(*it).first;
            a=temp.R;
            b=temp.G;
            c=temp.B;
            outFile.write(reinterpret_cast<const char*>(&a),sizeof(int));
            // outFile.write(reinterpret_cast<const char*>(&b),sizeof(int));
            // outFile.write(reinterpret_cast<const char*>(&c),sizeof(int));
            outFile.write(reinterpret_cast<char*>(&(*it).second),sizeof(int));
        }
        
        int len=code.length();
        char c=0;
        for(int i=0;i<len;i++){ 
            c <<= 1;
            if(code[i]=='1')
                c |= 1;
            if((i+1)%8==0){
                outFile.write(reinterpret_cast<char*>(&c),sizeof(char));
            }
        }
    }
    else{
        cout<<"�ݲ�֧�ֵ�����"<<endl;
        return;
    }
	
}

void Picture::decompress(FILE_NAME name){
    ifstream inFile(name+".hfm",ios::binary);
    if (!inFile.is_open())
    {
        cout<<"�ļ���ʧ��,�����ļ������Ƿ���ȷ"<<endl;
        return;
    }
    ofstream outFile(name+"_decom.ppm");
    if(!outFile.is_open()){
        cout<<"�����ļ��Ѿ����ڻ���Ȩ�޲����޷�����"<<endl;
        return;
    }

	int addition;
	int cnt;
	inFile.read(reinterpret_cast<char*>(&addition),sizeof(int));
//	cout<<"��ȡ��addition��"<<addition<<endl;
	
	inFile.read(reinterpret_cast<char*>(&cnt),sizeof(int));
//	cout<<"��ȡ��count��"<<cnt<<endl;
	

    int width,height,max_color;
    char type[3];
    inFile.read(type, 2);//����
    inFile.read(reinterpret_cast<char*>(&width),sizeof(int));//���
    inFile.read(reinterpret_cast<char*>(&height),sizeof(int));//�߶�
    inFile.read(reinterpret_cast<char*>(&max_color),sizeof(int));//���ֵ

    /*ͷ����Ϣд��*/
    outFile<<type<<endl<<width<<" "<<height<<endl<<max_color<<endl;
    string format=type;
    if(format=="P3"){
        RGB data;
        int pro;
        FrequencyMap.clear();
        for(int i=0;i<cnt;i++){
            int a,b,c;
            inFile.read(reinterpret_cast<char*>(&a),sizeof(int));
            inFile.read(reinterpret_cast<char*>(&b),sizeof(int));
            inFile.read(reinterpret_cast<char*>(&c),sizeof(int));
            RGB temp(a,b,c);
            inFile.read(reinterpret_cast<char*>(&pro),sizeof(int));
            // cout<<"��ȡ����ch:"<<a<<" "<<b<<" "<<c<<",��Ӧ��Ƶ����"<<pro<<endl; 
            FrequencyMap[temp]=pro;
        }
        
        BuildHuffmanTree();
        // getCodeTable();

        // for(auto it:CodeTable){
        //     cout<<it.first.R<<" "<<it.first.G<<" "<<it.first.B<<"��Ӧ�ı��룺"<<it.second<<endl;
        // }

        char c;
        string code="";
        while(inFile.get(c)){
            for(int i=0;i<8;i++){
                if((c&128)==128)
                    code+="1";
                else
                    code+="0";
                c<<=1;
            }
        }
        
        // cout<<"��ѹ��ı��룺"<<code<<endl;

        int len=code.length();
        TreeNode* cur=root;
        for(int i=0;i<=len-addition;){
            if(cur->left||cur->right){
                if(code[i]=='0')
                    cur=cur->left;
                else
                    cur=cur->right;
                i++;
            }
            else{
                outFile<<cur->data.R<<" "<<cur->data.G<<" "<<cur->data.B<<" ";
                cur=root;
            }
        }
    }
    else if(format=="P2"){
        RGB data;
        int pro;
        FrequencyMap.clear();
        for(int i=0;i<cnt;i++){
            int a;
            inFile.read(reinterpret_cast<char*>(&a),sizeof(int));
            // inFile.read(reinterpret_cast<char*>(&b),sizeof(int));
            // inFile.read(reinterpret_cast<char*>(&c),sizeof(int));
            RGB temp(a,-1,-1);
            inFile.read(reinterpret_cast<char*>(&pro),sizeof(int));
            // cout<<"��ȡ����ch:"<<a<<" "<<b<<" "<<c<<",��Ӧ��Ƶ����"<<pro<<endl; 
            FrequencyMap[temp]=pro;
        }
        
        BuildHuffmanTree();
        // getCodeTable();

        // for(auto it:CodeTable){
        //     cout<<it.first.R<<" "<<it.first.G<<" "<<it.first.B<<"��Ӧ�ı��룺"<<it.second<<endl;
        // }

        char c;
        string code="";
        while(inFile.get(c)){
            for(int i=0;i<8;i++){
                if((c&128)==128)
                    code+="1";
                else
                    code+="0";
                c<<=1;
            }
        }
        
        // cout<<"��ѹ��ı��룺"<<code<<endl;

        int len=code.length();
        TreeNode* cur=root;
        for(int i=0;i<=len-addition;){
            if(cur->left||cur->right){
                if(code[i]=='0')
                    cur=cur->left;
                else
                    cur=cur->right;
                i++;
            }
            else{
                outFile<<cur->data.R<<" ";
                cur=root;
            }
        }
    }
	

}

bool Picture::check(FILE_NAME name,int x){
    switch(x){
        case 0:{
            ifstream in(name+".ppm");
            if(!in.is_open())
                return false;
            else
                return true;
            in.close();
            break;
        }
        case 1:{
            ifstream in(name+".hfm");
            if(!in.is_open())
                return false;
            else
                return true;
            in.close();
            break;
        }
        default:{
            return false;
        }
    }
    return false;
}