echo 'Working directory'
read dirname #directory 이름 입력을 저장

if [ -n "$dirname" ];then #빈문자열이 아니라면
        if [ ! -d "$dirname" ];then #디렉토리가 존재하지 않는다면
                echo "$dirname is not exist"
        elif [ ! -x "$dirname" ];then #실행권한이 없다면
                echo "$dirname is inaccessible"
        else #디렉토리가 존재하며 실행할 수 있다면
                cd $dirname
        fi
else #빈문자열이라면
        echo "program starts in the current directory"
fi

for dir in *
        do
        newname=`echo $dir | tr '[a-z][A-Z]' '[A-Z][a-z]'`
        #변수 newname은 dir의 대소문자를 tr을 이용해서 바꾼것        
        mv $dir $newname #$dir을 newname으로 바꾼다.        
done
