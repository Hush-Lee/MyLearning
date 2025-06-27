counter=1
for file in *; do
    if [ -f "$file" ]; then
        # 保留原扩展名，按数字顺序重命名（例如：file_1.txt）
        newname="$1$counter.${file##*.}"
        
        # 如果要完全替换名称（不保留扩展名）：
        # newname="new_name_$counter"
        
        # 执行重命名（测试时建议先加上echo预览）
        mv -- "$file" "$newname"
        ((counter++))
    fi
done
