CONVERTIMG = ConvertImg
CONVERTFLAGS = --format jpgdata --compression 40

IMAGE_OBJS = $(patsubst %.bmp,%.h,$(wildcard *.bmp))
IMAGE_JPGS = $(patsubst %.bmp,%.jpg,$(wildcard *.bmp))

%.h: %.png
	$(CONVERTIMG) $(CONVERTFLAGS) $^ $@

%.png: %.bmp
	convert -resize 160x120 $^ $@

#all: $(IMAGE_JPGS)
all: $(IMAGE_OBJS)

clean:
	rm -f *.h
