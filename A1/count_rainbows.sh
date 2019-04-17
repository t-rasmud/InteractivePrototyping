while true
do
	grep -wr "Rainbow" rnb.txt | wc -w > rnb_cnt.txt
        sleep 10
done
