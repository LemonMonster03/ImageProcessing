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
        cout<<"文件打开失败,请检查文件名字是否正确"<<endl;
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
        cout<<"暂不支持的格式"<<endl;
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
        cout<<"文件打开失败,请检查文件名字是否正确"<<endl;
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
        cout<<"图片已保存为 "<<temp<<endl;
        imshow("Picture_Gray",gray_img);
        waitKey(0);
    }
    else if(format=="P2"){
        cout<<"图片已经是灰度图"<<endl;
        return;
    }
    else{
        cout<<"暂不支持的格式"<<endl;
        return;
    }
}

void Picture::resize_img(FILE_NAME name){
    ifstream ppm_file(name + ".ppm");

    if (!ppm_file.is_open())
    {
        cout<<"文件打开失败,请检查文件名字是否正确"<<endl;
        return;
    }
    string format;
    ppm_file >> format;
    int w,h;
    cout<<"请输入缩放后的高度：";
    cin>>h;
    cout<<"请输入缩放后的高度：";
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
        cout<<"图片已保存为 "<<temp<<endl;
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
        cout<<"图片已保存为 "<<temp<<endl;
        imwrite(temp,des_img);
        imshow("Picture_resize",des_img);
        waitKey(0);
    }
    else{
        cout<<"暂不支持的格式"<<endl;
        return;
    }
}



bool Picture::getCount(FILE_NAME name){
    ifstream ppm_file(name + ".ppm");

    if (!ppm_file.is_open())
    {
        cout<<"文件打开失败,请检查文件名字是否正确"<<endl;
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
        cout<<"暂不支持的格式"<<endl;
        return false;
    }
    return true;
}

void Picture::BuildHuffmanTree(){
	/*为所有字符构建节点，并且全部入队*/
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
    //     cout<<it.first.R<<" "<<it.first.G<<" "<<it.first.B<<"对应的编码："<<it.second<<endl;
    // }
}

void Picture::dfs(TreeNode* root,string code){
	if(!root)
		return;
	if((!root->left)&&(!root->right)){
		CodeTable[root->data]=code;
		// cout<<"数据"<<root->data.R<<" "<<root->data.G<<" "<<root->data.B<<"的编码："<<code<<endl;
		return;
	}
	dfs(root->left,code+"0");
	dfs(root->right,code+"1");
}

void Picture::compress(FILE_NAME name){
    ifstream inFile(name+".ppm");
    if (!inFile.is_open())
    {
        cout<<"文件打开失败,请检查文件名字是否正确"<<endl;
        return;
    }
    ofstream outFile(name+".hfm",ios::binary);
    if(!outFile.is_open()){
        cout<<"错误，文件已经存在或者权限不足无法创建"<<endl;
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
        // cout<<"对文本压缩后的编码："<<code<<endl;
        
        addition=code.length()%8 ? (8-code.length()%8) : 0;//附加的0的个数 
    //	cout<<"应该附加的0的个数："<<addition<<endl; 
        
        for(int i=0;i<addition;i++){
            code+="0";
        }
        // cout<<"对文本压缩后的编码："<<code<<endl;

        /*写入附加0个数*/
        outFile.write(reinterpret_cast<char*>(&addition), sizeof(int));

    //	cout<<"字符的个数："<<count<<endl; 
        /*写入RGB元素个数*/
        outFile.write(reinterpret_cast<char*>(&count), sizeof(int));

        outFile.write(type.c_str(), type.length());//类型
        outFile.write(reinterpret_cast<char*>(&width),sizeof(int));//宽度
        outFile.write(reinterpret_cast<char*>(&height),sizeof(int));//高度
        outFile.write(reinterpret_cast<char*>(&max_color),sizeof(int));//最大值
        
        /*将RGB以及对应频率写入文件*/
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

        // cout<<"对文本压缩后的编码："<<code<<endl;
        
        addition=code.length()%8 ? (8-code.length()%8) : 0;//附加的0的个数 
    //	cout<<"应该附加的0的个数："<<addition<<endl; 
        
        for(int i=0;i<addition;i++){
            code+="0";
        }
        // cout<<"对文本压缩后的编码："<<code<<endl;

        /*写入附加0个数*/
        outFile.write(reinterpret_cast<char*>(&addition), sizeof(int));

    //	cout<<"字符的个数："<<count<<endl; 
        /*写入RGB元素个数*/
        outFile.write(reinterpret_cast<char*>(&count), sizeof(int));

        outFile.write(type.c_str(), type.length());//类型
        outFile.write(reinterpret_cast<char*>(&width),sizeof(int));//宽度
        outFile.write(reinterpret_cast<char*>(&height),sizeof(int));//高度
        outFile.write(reinterpret_cast<char*>(&max_color),sizeof(int));//最大值
        
        /*将RGB以及对应频率写入文件*/
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
        cout<<"暂不支持的类型"<<endl;
        return;
    }
	
}

void Picture::decompress(FILE_NAME name){
    ifstream inFile(name+".hfm",ios::binary);
    if (!inFile.is_open())
    {
        cout<<"文件打开失败,请检查文件名字是否正确"<<endl;
        return;
    }
    ofstream outFile(name+"_decom.ppm");
    if(!outFile.is_open()){
        cout<<"错误，文件已经存在或者权限不足无法创建"<<endl;
        return;
    }

	int addition;
	int cnt;
	inFile.read(reinterpret_cast<char*>(&addition),sizeof(int));
//	cout<<"读取的addition："<<addition<<endl;
	
	inFile.read(reinterpret_cast<char*>(&cnt),sizeof(int));
//	cout<<"读取的count："<<cnt<<endl;
	

    int width,height,max_color;
    char type[3];
    inFile.read(type, 2);//类型
    inFile.read(reinterpret_cast<char*>(&width),sizeof(int));//宽度
    inFile.read(reinterpret_cast<char*>(&height),sizeof(int));//高度
    inFile.read(reinterpret_cast<char*>(&max_color),sizeof(int));//最大值

    /*头部信息写入*/
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
            // cout<<"读取到的ch:"<<a<<" "<<b<<" "<<c<<",对应的频数："<<pro<<endl; 
            FrequencyMap[temp]=pro;
        }
        
        BuildHuffmanTree();
        // getCodeTable();

        // for(auto it:CodeTable){
        //     cout<<it.first.R<<" "<<it.first.G<<" "<<it.first.B<<"对应的编码："<<it.second<<endl;
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
        
        // cout<<"解压后的编码："<<code<<endl;

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
            // cout<<"读取到的ch:"<<a<<" "<<b<<" "<<c<<",对应的频数："<<pro<<endl; 
            FrequencyMap[temp]=pro;
        }
        
        BuildHuffmanTree();
        // getCodeTable();

        // for(auto it:CodeTable){
        //     cout<<it.first.R<<" "<<it.first.G<<" "<<it.first.B<<"对应的编码："<<it.second<<endl;
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
        
        // cout<<"解压后的编码："<<code<<endl;

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