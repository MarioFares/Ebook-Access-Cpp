# Project Documentation
<hr>

## Table of Contents
- [Demo](#demo)
- [Some Specifications](#specs)
	+ [Details](#details)
	+ [Notes](#notes)
- [Setup Environment](#setup)
	+ [Qt Installer](#install)
	+ [Setup IDE & Compilation](#compile)
	+ [Compiled Output](#output)
	+ [Implementation Directory](#implementation)
- [Application Manual](#manual)
	+ [Adding Books](#add)
		* [Single File](#single)
		* [Multi File](#multi)
	+ [Searching Ebooks](#search)
		* [Titles](#titles)
		* [Criteria](#criteria)
		* [Saving Searches](#save)
		* [Extended Searches](#ext)
	+ [Sorting Ebooks](#sort)
	+ [Updating MetaData](#meta)
	+ [Other Functions](#other)
		* [Cleaning Ebooks](#clean)
		* [Interface Customization](#custom)
		* [Resetting Data](#reset)
		* [Shortcuts](#shortcuts)

## Demo <a name="demo"></a>
Please watch my project [demo](https://www.youtube.com/watch?v=XBMX4xLqcjA).

## Some Specifications <a name="specs"></a>
This project is mainly a Windows Desktop application that uses the following technologies:
- C++
- Qt6 (GUI Framework)
- SQLite3 Database

Project Details<a name="details"></a>
- Author: Mario Fares
- Title: Ebook Access 
- Version: v2.0
- Architecture: x64

Notes:<a name="notes"></a>
- There are no third-party libraries that the user needs to install. The project is set up in a way such that if you follow the instructions and choose the right options in the Qt Installer, you can immediately compile the project with no hassles.

## Setup Environment<a name="setup"></a>

### Qt Installer<a name="install"></a>
We must first set up our dev environment. Please follow the steps below:
1. Go to [Qt Online Installer Page](https://www.qt.io/download-open-source?hsCtaTracking=9f6a2170-a938-42df-a8e2-a9f0b1d6cdce%7C6cb0de4f-9bb5-4778-ab02-bfb62735f3e5)
2. Scroll down until you find `Download the Qt Online Installer`
3. Click Download
4. Once the installer is downloaded run it and follow the below steps
	1. Create a Qt Account and Login
	2. Agree to the terms and conditions
	3. Disable sending statistics
	4. Choose an installation folder
	5. Choose `Custom Installation` 
	6. In `Qt` > `Qt 6.1.3` select the following options:
		1. `MinGW 8.1.0 64-bit`
		2. `Qt5 Compatibility Module`
		3. `Additional Libraries`
	4. Follow the rest of the instructions in the installer
	5. At the end of the installer, launch `Qt Creator` IDE
		

### Setup IDE & Compilation <a name="compile"></a>
The IDE that we will be using is the Qt Creator IDE. Once you complete the above installation steps, you will now have the Qt Creator IDE on your desktop.
1. Launch Qt Creator
2. Have the implementation folder ready
3. In Qt Creator, select `Projects` on the upper left corner
4. Click on Open
5. Proceed to find the `.pro` file in the implementation folder
6. When prompted to configure the project, select the kit displayed. (there should only be one)
6. Once all is set up, you can now click Run (choose release instead of debug) or Build on the bottom left of the IDE.
7. The application should compile with no problem and run itself.


### Compiled Output<a name="output"></a>
- Find the compiled application in a generated directory found in the parent directory of the implementation directory (i.e. go to the parent directory of the directory containing the .pro file)
- Go to the `release` directory
- Executable file should be there


### Around the Implemenation Directory <a name="implementation"></a>
Please find descriptions for the following in the implemenation directory:
- `docs`: Contains all documentation files
- `forms`: Contains .ui files for GUI
- `headers`: contains .h header files
- `resources`: contains icons and stylesheets
- `source`: contains .cpp source files
- `EbookAccess.pro`: specific to the IDE

## Application Manual<a name="manual"></a>

### Adding your ebooks<a name="add"></a>
In the bottom left corner of the application, there are 2 buttons that resemble books, the left one is to add a single book, and the right one is to choose a directory from which you can recursively add all the books inside.

#### Single File Function<a name="single"></a>
Again, the left button will launch a dialog where you can select and add a single file.
You can use the `...` button to find a file. This method is superior to adding all the file's details manually since when you select a file, the `name`, `folder`, `path`, `extension`, and `size` fields will automatically be filled out.

#### Multi-File Function<a name="multi"></a>
The right button mentioned above will help in adding multiple files recursively. In this dialog as well, there is a `...` button to choose the preferred directory.

If you check the recursive checkbox, then the algorithm will dig into every subdirectory in the given directory and retrieve all files. Please note depending on the number of files, the function could be slow.

Click `add` and follow the instructions. Once the progress bar hits 100%, you are free to close the dialog box.

### Searching your ebooks<a name="search"></a>
#### Titles<a name="titles"></a>
As you see on the right side of the interface, there is a search bar, which automatically updates titles as you type. The search bar will return the names of the ebooks that contain whatever you typed into the bar. You can also clear and sort your selections alphabetically.

#### Criteria<a name="criteria"></a>
You can search your ebooks using the following criteria:
- `Folder`: This is simply the parent directory in which files are located in
- `Author`: You must set an ebook's author manually
- `Genre`: You must set an ebook's genre manually
- `Tags`: You can input tags that are associated with files
- `Extensions`: you can search ebooks based on their extensions (ex: '.pdf')
- `Size`: You can specify the size range you are searching for. You can also choose the unit (KB, MB, GB)
- `Pages`: You can search a range of pages, but be careful, you must manually update a book's pages

#### Saving a search<a name="save"></a>
You can save all the criteria you inputted by clicking on the save button.

Then, you can select a search from the `Search` combo box and press the load button.

#### Extended Selection Search<a name="ext"></a>
As you see, there is a small button next to every search criteria. This should be the preferred method to choose for example multiple folders or extensions.

This button will show you all the existing data for a certain criterium. Once you select your options and click `Continue`, you will see the relative box updated.

### Sorting your ebooks<a name="sort"></a>
You can sort your ebooks based on all the criteria you use to search them.

The 3 most important sorting mechanisms are `Author`, `Genre`, and `Tags`.

Please note the application uses the `|` character as its universal separator. Therefore when searching tags or updating an ebook's tags, you must separate every tag with a `|` character.

### Updating MetaData<a name="meta"></a>
Once you click on an ebook displayed in the List Widget (the frame in the top right), its metadata will automatically be shown in the Details Frame (the bottom right frame).


From there, you can change any metadata you want except the `extension` and `size`. 

Input your author, genre, and tags for a specific ebook and press on `Update`. If you messed around with the data but did NOT press `Update`, you can use `Restore`.
### Other Functions<a name="other"></a>
#### Cleaning Ebooks<a name="clean"></a>
One of the fundamental philosophies of this application is to work with files in the database and not on the hard disk. What does that mean?

The application does not copy or move any files on disk, but it simply stores all the data it needs in a database. Why is this relevant to the clean function? What if you deleted your file on the hard disk? This would not automatically delete it from your application and therefore you would use the clean function.


What does the clean function do? It simply iterates over all the file paths listed in the database and checks if that path is still valid.
#### Interface Customization<a name="custom"></a>
You cannot customize the colors	of the interface but you can however hide any frame you do not want to see.

Use the `View` menu to see everything you can show and hide.
#### Resetting Data<a name="reset"></a>
In the `Reset` menu, you are able to reset many things including and limited to:
- Tags
- Ebooks
- Searches
- Database (this resets all the above)
#### Shortcuts<a name="shortcuts"></a>
For added workflow productivity, every action in the menus has a shortcut. Therefore you are encouraged to look over all the shortcuts, choose which ones are most useful to learn.

The shortcut naming system is based on the first letter of each word of the behavior. Example: `Search Files` = `Ctrl+S, Ctrl+F`. Hold down the Ctrl key and press S then F.