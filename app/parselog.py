import os
import sys
import subprocess
from PIL import Image

WIDTH = 36
HEIGHT = 42
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
COLORS = [BLACK, WHITE]
NEXT_COLOR = {BLACK:WHITE, WHITE:BLACK}
OFFSET = ord('"')
MAX_SIZE = ord('~')-ord('"')

def parse(data):    
    data = list(data)
    assert data[0] == data[-1] == '!'
    data = data[1:-1]
    #seconds = ord(data.pop(0)) - OFFSET
    #mseconds = ord(data.pop(0)) - OFFSET
    color = COLORS[int(data.pop(0))-1]

    pixels = {}
    offset = 0
    while data:
        sz = ord(data.pop(0)) - OFFSET
        
        for i in range(offset, offset + sz):
            pixels[(i % WIDTH, i / WIDTH)] = color
            
        offset += sz
        if sz != MAX_SIZE:
            color = NEXT_COLOR[color]
    assert offset == (WIDTH * HEIGHT), 'total size mismatch got %d exp %d' % (offset, WIDTH * HEIGHT)
    #return seconds + float(mseconds) / 100, pixels
    return pixels

def log2img(idx, data):
    im = Image.new('RGB', (WIDTH*4, HEIGHT*4))

    try:
        #sec, pix = parse(data)
        pix = parse(data)
    except Exception as e:
        print 'Error parsing: %s: %s' % (e, data)
        raise

    for y in range(HEIGHT):
        for x in range(WIDTH):
            color = pix[x,y]
            for xx in range(x*4, x*4+3):
                for yy in range(y*4, y*4+3):
                    im.putpixel((xx,yy), color)

    fname = 'screenshot_%04d.png' % idx
    #im.save('screenshot_%05.2f.png' % sec)
    im.save(fname)
    print 'saved %s' % fname

def img2gif(paths):
    # assert all(path.startswith('screenshot_') and path.endswith('.png') for path in paths)
    # last_sec = None
    # convert = r'c:\Program Files (x86)\ImageMagick-6.8.4-Q16\convert.exe'
    # if not os.path.exists(convert):
    #     convert = 'convert'
    # args = [convert, '-loop', '0']
    # for path in paths:
    #     sec = float(path[:-4].split('_')[1])
    #     if last_sec is not None:
    #         delay = sec - last_sec
    #         args += ['-delay', str(int(delay*100))]
    #     args += [path]
    #     last_sec = sec
    # args += ['screenshot.gif']
    # print ' '.join(args)
    # subprocess.check_call(args)

    assert all(path.startswith('screenshot_') and path.endswith('.png') for path in paths)
    convert = r'c:\Program Files (x86)\ImageMagick-6.8.4-Q16\convert.exe'
    if not os.path.exists(convert):
        convert = 'convert'
    args = [convert, '-loop', '0', '-delay', '10'] + paths + ['screenshot.gif']
    print ' '.join(args)
    subprocess.check_call(args)

if len(sys.argv) > 1:
    f = open(sys.argv[1])
else:
    f = sys.stdin

idx = 0
image_lines = None
for line in f:
    line = line.strip().split()
    if not line:
        continue
    
    line = line[-1]
    if image_lines:
        image_lines.append(line)
        if line.endswith('!'):
            try:
                log2img(idx, ''.join(image_lines))
            except:
                pass
            idx += 1
            image_lines = None
        
    elif line.startswith('!'):
        image_lines = [line]

paths = sorted(os.listdir('.'))
paths = [p for p in paths if p.startswith('screenshot_')]
img2gif(paths)
