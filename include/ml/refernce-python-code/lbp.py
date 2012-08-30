"""
http://www.ee.oulu.fi/mvg/page/lbp_matlab
"""

import math
import numpy

spoints = [[-1, -1],[-1, 0], [-1, 1], [0, -1], [-0, 1], [1, -1],[1, 0], [1, 1]]


class LBP(object):
    def __init__(self):
        pass

    def __del__(self):
        pass

    def init2(self, radius, neighbors):
        # Angle step.
        a = 2*math.pi/neighbors;
        theta = numpy.arange(0,2*math.pi,a)
        spoints = [-numpy.sin(theta),numpy.cos(theta)]
        spoints = numpy.array(spoints).T
        spoints *= radius
        self.init(spoints)


    def init(self, spoints):
        self.spoints = spoints
        ((origy, origx),(dy,dx)) = init(self.image, spoints)
        self.orig = numpy.array((origy,origx))
        self.dsize = numpy.array((dy,dx))
        # Fill the center pixel matrix C.
        self.C = self.image[origy:origy+dy,origx:origx+dx]
        #d_C = double(C)
        ##bins = 2^neighbors;
        ## Initialize the result matrix with zeros.
        self.result = numpy.zeros((dy,dx))



    def compute(self):
        for i,p in enumerate(self.spoints):
            y,x = p + self.orig
            N = lbp(self.image, (y,x), self.dsize)
            D = N >= self.C
            #Update the result matrix.
            v = 1<<i
            self.result += v*D




def init(image, spoints):
    #Determine the dimensions of the input image.
    [ysize, xsize] = image.shape
    miny=min(spoints[:,0])
    maxy=max(spoints[:,0])
    minx=min(spoints[:,1])
    maxx=max(spoints[:,1])

    # Block size, each LBP code is computed within a block of size bsizey*bsizex
    bsizey = math.ceil(max(maxy,0)) - math.floor(min(miny,0)) + 1
    bsizex = math.ceil(max(maxx,0)) - math.floor(min(minx,0)) + 1

    # Coordinates of origin (0,0) in the block
    origy =  0 - math.floor(min(miny,0))
    origx =  0 - math.floor(min(minx,0))

    # Minimum allowed size for the input image depends on the radius of the used LBP operator.
    if (xsize < bsizex) or (ysize < bsizey):
        print 'Too small input image. Should be at least (2*radius+1) x (2*radius+1)';
        return

    # Calculate dx and dy;
    dx = xsize - bsizex + 1
    dy = ysize - bsizey + 1

    return ((origy, origx),(dy,dx))




def lbp(image, orig, dsize):
    y,x = orig
    dy,dx = dsize
    # Calculate floors, ceils and rounds for the x and y.
    fy = math.floor(y); cy = math.ceil(y); ry = round(y);
    fx = math.floor(x); cx = math.ceil(x); rx = round(x);
    # Check if interpolation is needed.
    if (abs(x - rx) < 1e-6) and (abs(y - ry) < 1e-6):
        # Interpolation is not needed, use original datatypes
        N = image[ry:ry+dy,rx:rx+dx]
    else:
        # Interpolation needed, use double type images
        ty = y - fy
        tx = x - fx
        # Perform linear interpolation.
        w1 = (1 - tx) * (1 - ty)
        w2 =      tx  * (1 - ty)
        w3 = (1 - tx) *      ty
        w4 =      tx  *      ty
        # Compute interpolated pixel values
        N = w1*image[fy:fy+dy,fx:fx+dx]
        N += w2*image[fy:fy+dy,cx:cx+dx]
        N += w3*image[cy:cy+dy,fx:fx+dx]
        N += w4*image[cy:cy+dy,cx:cx+dx]
    return N



