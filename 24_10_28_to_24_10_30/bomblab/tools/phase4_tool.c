# include <stdio.h>
# include <math.h>

long long int tool(long long int num){
    if(num==0){
        return 1;
    }else{
        long long int num_ = num/2;
        long long ret = tool(num_);
        if(num%2==0){
            return ret*ret;
        }else{
            return ret*ret*8;
        }
    }
}

int main(){
    int num;

    // 输入
    // scanf("%d", &num);
    // tool(num);

    // 遍历
    for(long long int i = 0; i < 4294967295; i++){
        long long int ret = tool(i);
        if(ret==1073741824){
            printf("\n\n\n%lld 可以！！！\n\n\n",i);
            getchar();
        }else{
            printf("%lld no ",i);
        }
    }

    return 0;
}