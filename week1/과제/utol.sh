echo 'Working directory'
read dirname #directory �̸� �Է��� ����

if [ -n "$dirname" ];then #���ڿ��� �ƴ϶��
        if [ ! -d "$dirname" ];then #���丮�� �������� �ʴ´ٸ�
                echo "$dirname is not exist"
        elif [ ! -x "$dirname" ];then #��������� ���ٸ�
                echo "$dirname is inaccessible"
        else #���丮�� �����ϸ� ������ �� �ִٸ�
                cd $dirname
        fi
else #���ڿ��̶��
        echo "program starts in the current directory"
fi

for dir in *
        do
        newname=`echo $dir | tr '[a-z][A-Z]' '[A-Z][a-z]'`
        #���� newname�� dir�� ��ҹ��ڸ� tr�� �̿��ؼ� �ٲ۰�        
        mv $dir $newname #$dir�� newname���� �ٲ۴�.        
done
