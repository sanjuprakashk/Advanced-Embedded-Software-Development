if [ -f info.txt ]; then # If info.txt already exists, delete it
   rm info.txt
fi
echo -n "User Information : " >> info.txt | id >> info.txt 
echo -e "\n" >> info.txt
echo -e "-----------------------------------------------------------------------------------------------------------\n" >> info.txt

echo -n "Operating system type/brand : " >> info.txt | uname -o >> info.txt
echo -e "\n" >> info.txt
echo -e "-----------------------------------------------------------------------------------------------------------\n" >> info.txt

echo -n "OS Distribution : " >> info.txt | lsb_release -d >> info.txt
echo -e "\n" >> info.txt
echo -e "-----------------------------------------------------------------------------------------------------------\n" >> info.txt

echo -n "OS Version : " >> info.txt | cat /etc/*-release | grep "VERSION=" >> info.txt
echo -e "\n" >> info.txt
echo -e "-----------------------------------------------------------------------------------------------------------\n" >> info.txt

echo -n "Kernel Version : " >> info.txt | uname -r >> info.txt
echo -e "\n" >> info.txt
echo -e "-----------------------------------------------------------------------------------------------------------\n" >> info.txt

echo -n "Kernel gcc version build : " >> info.txt | gcc --version | grep gcc >> info.txt
echo -e "\n" >> info.txt
echo -e "-----------------------------------------------------------------------------------------------------------\n" >> info.txt

echo -n "Kernel Build Time : " >> info.txt | uname -v >> info.txt
echo -e "\n" >> info.txt
echo -e "-----------------------------------------------------------------------------------------------------------\n" >> info.txt

echo "System Architecture Information : " >> info.txt
lscpu  >> info.txt
echo -e "\n" >> info.txt 
echo -e "-----------------------------------------------------------------------------------------------------------\n" >> info.txt

echo "Information on File System Memory : " >> info.txt
df -h  >> info.txt
echo -e "\n" >> info.txt 
echo -e "-----------------------------------------------------------------------------------------------------------\n" >> info.txt
