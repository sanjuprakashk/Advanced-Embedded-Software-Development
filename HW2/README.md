Problem2: Run make PLATFORM=HOST to get an executable to run on native linux

Problem4: Run make PLATFORM=ARM to get an executable to run on the BeagleBone Green

Problem6: To start a CRON Task do the following:  
          1) From root run crontab -e  
          2) In the editor type - */10 * * * * /usr/bin/./kernel_sort_cron >> log.txt and save.  
