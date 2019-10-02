## Q1:
-   One command line argument is required: 
	`./a.out <filePath>`


-	If More or Less arguments are given then program will give ERROR.

-	"write" function is used to write anything on terminal.

-	"len" function is used to calculate the length of the string.

-	File is opened with readOnly mode using "open" command and program returns error using "perror" if file can't be opened may be due to wrong filePath.

-	"Stat" is used to check if "Assignment" folder exist. If exists then it's permission is changed to 0700 using "chmod" otherwise newFolder is created using "mkdir" with permission 0700.

-	Name of file is fetched from `<filePath>` and concatenated using "cat" function (at the top of "main" function) after "Assignment" to get the address of newFile and if that file already exists then it's deleted using "remove" and then created using "open" with writeOnly mode with permission 0600.

-	Now "lseek" is used to read from end of file and max_buffer_size is 1e6. Buffer string is reversed and written to the new file using "write" and percentage is calculated and overwritten on the previous percentage using "\r".

-	Both files are closed.



## Q2:
-	Three command line arguments are required:
	`./a.out <newfilePath> <oldfilePath> <directoryPath>`


-	If More or Less arguments are given then program will give ERROR.

-	"write" function is used to write anything on terminal.

-	"len" function is used to calculate the length of the string.

-	"permissionCheck" function checks the permission of the files if they EXIST using "STAT" command which stores the information of that file including permissions in "fileStat" variable.

-	"isReversed" function takes two arguments `<newFilePath>` and `<oldFilePath>` and if the address is correct then it opens both file with readOnly mode and get the size of both files in bytes and if they are equal then "lseek" is used to read from the starting from one file and from end from another file with max_buffer_size of 1e6 and checks if strings are reversed and 1 is returned if it's correctly reversed else 0 and both files are closed.

-	How much percent of file is checked is shown on terminal using "write" function and overwritten using "\b".