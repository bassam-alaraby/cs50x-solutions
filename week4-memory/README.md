# Week 4 — Memory

**Topic**: Pointers, memory allocation, file I/O, binary data

## Problems

### Volume
Scales the volume of a WAV audio file by a given factor.
Reads the header as-is, then multiplies each audio sample by the factor.

### Filter (less)
Applies image filters to BMP files: grayscale, sepia, reflection, and blur.
Each filter modifies pixel RGB values following standard formulas.

### Recover
Recovers deleted JPEGs from a raw memory card image (card.raw).
Reads 512-byte blocks, detects JPEG headers, and writes each image to a new file.
Recovered 50 JPEG files (000.jpg → 049.jpg).
