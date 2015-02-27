# what
Batch creation of standart freedesktop thumbnails, compatible with thunar, pcmanfm, nautilus etc

# why
Mainly to generate thumbnail(s) from the script or to overcome file browser failures

# how
There are 3 steps. 

First you need to install programs that actually create thumbnails

 * video - ffmpegthumbnailer
 * pdf - ghostscript or evince
 
Second, create mapping betwean mime types and thumbnailers. These mappings are text files that reside in
`/usr/share/thumbnailers` dir and have '.thumbnailer' extension. For example

File `/usr/share/thumbnailers/video.thumbnailer`
```
[Thumbnailer Entry]
TryExec=ffmpegthumbnailer
Exec=ffmpegthumbnailer -s %s -i %u -o %o -c png -f -t 10
MimeType=video/flv;video/webm;video/mkv;video/mp4;video/mpeg;video/avi;video/ogg;video/quicktime;video/x-avi;video/x-flv;video/x-mp4;video/x-mpeg;video/x-webm;video/x-mkv;application/x-extension-webm;video/x-matroska;video/x-ms-wmv;video/x-msvideo;video/x-msvideo/avi;video/x-theora/ogg;video/x-theora/ogv;video/x-ms-asf;video/x-m4v;
```

Third, run the script :-)
```
thumbnailer ~/torrents/*.mkv
```
