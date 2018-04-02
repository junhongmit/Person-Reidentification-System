##################################################################
# 1. 输入单张图片，输出top15的图片
run:
python test_single.py --pic_path (path of a single picture)

example:
python test_single.py --pic_path dataset/test_query/-1/015995.jpg

在result_JPG文件夹里查看生成结果
'0_xxx.jpg'是query原图
'1_xxx.jpg'~'15_xxx.jpg'是Top15的结果

##################################################################
# 2. 输入query数据集，输出xml文件
run:
python test_multiple.py --query_dir (path of query dataset)

example:
python test_multiple.py --query_dir dataset/test_query

在result_XML文件夹里查看生成结果


