# LCDmonodlp
Use a high resolution monochrome LCD to expose objects

----------

<h4> Tool for uv/light exposing objects via a high resolution (4K) monochrome LCD.(dlp method)</h4>

---

Created with the purpose of placing a soldermask on milled out pcb's

Initial file: a 1:1 pdf file (Adafruit JTAG to SWD Adapter_for_print-F_Paste.pdf)

Input: a raw monochrome pbm file (P4)(in monochrome LCD subpixel resolution)

Output: a subpixel separated ppm image file. (fb.ppm)(P5)

# Setup:

    git clone https://github.com/the-snowwhite/LCDmonodlp.git
    cd LCDmonodlp
    make                   (compiles file)
    Sudo make install      (installs binary to /usr/bin
    
if you wish to uninstall again type:

    sudo make uninstall

# usage

    ppmmonodlp infile.pbm invert(int) outfile.ppm

Whole Method:
===
Kicad:
---
- Use edit worksheet tool: create a new worksheet,draw a retangle border alone the outside, set all margins to 0.

- Goto page settings in pcbnew:

- edit pagesize

    - make custom page size same size as screen (Height: 120mm, Width:192 mm), import the worksheet you just created.

- Place your pcb in one corner (like left bottom) and set origins in that corner, create a text in user layer at opposite corner.

- Plot the layer you want to expose as a pdf (ie: F.Paste), use only mirrored and inverted options.

Convert your pdf to a monochrome pbm file
===
\-g is the height and width of your monochrome LCD display in subpixels

\-r is the subfixel dpi of your monochrome LCD display

>    gs -dNORANGEPAGESIZE -sOutputFile=print.pbm -dNOPAUSE -dBATCH -g3840x2400 -r508 -sDEVICE=pgnmraw -dSAFER 'Adafruit JTAG to SWD Adapter_for_print-F_Paste.pdf'

Convert the pbm file into a ppm file:
---
(where the former pixels are separated out into r,g,b subpivels.)

>   ppmmonodlp print.pbm 0 fb.ppm

(source in this repo)

Copy the file to embedded system via ssh (optional)
---

>    sshpass -p holosynth scp fb.ppm holosynth@holosynthv-u96:~


Display image on LCD :
---
(via ssh terminal)

>    DISPLAY=:0 sudo fbi -T 1 fb.ppm
