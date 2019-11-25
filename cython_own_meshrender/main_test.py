import numpy as np
import cv2
import mesh_render
import os
import codecs

class OBJ:
    def __init__(self, fdir, filename):
        if filename==None: return
        self.vertices = []
        self.normals = []
        self.texcoords = []
        self.faces = []
        self.v_colors=[]
        self.mtl = None

        OBJ_path = os.path.join(fdir + filename)
        obj_f= codecs.open(OBJ_path, 'r',encoding= 'utf-8',errors='ignore')
    
        
        for line in obj_f:
            if line.startswith('#'): continue
            values = line.split()
            if not values: continue

            if values[0] == 'mtllib':
                self.mtl = [fdir,values[1]]
            elif values[0] == 'v':
                v = [float(x) for x in values[1:4]]
                self.vertices.append(v)

                if len(values)==7:
                    # c = [float(x) for x in values[4:7]]
                    # self.v_colors[ len(self.vertices)-1 ] = c
                    self.v_colors.append(values[4:7])
                    material = None

            elif values[0] == 'vn':
                # v = list(map(float, values[1:4]))
                v = [float(x) for x in values[1:4]]
                self.normals.append(v)

            elif values[0] == 'vt':
                v = [float(x) for x in values[1:3]]
                self.texcoords.append(v)

            elif values[0] in ('usemtl', 'usemat'):
                material = values[1]

            elif values[0] == 'f':
                face_inds = []
                texture_coords = []
                norms = []
                for v in values[1:]:
                    w = v.split('/')
                    face_inds.append(int(w[0]))
                    if len(w) >= 2 and len(w[1]) > 0:
                        texture_coords.append(int(w[1]))
                    else:
                        texture_coords.append(0)
                    if len(w) >= 3 and len(w[2]) > 0:
                        norms.append(int(w[2]))
                    else:
                        norms.append(0)      
                self.faces.append((face_inds, norms, texture_coords, material))



def render_colors(vertices, triangles, colors, image):
    # colors = colors *(1/255)
    h = image.shape[0]
    w = image.shape[1]
    c = image.shape[2]

    black_map = np.zeros((h, w, c),dtype = np.float32)  
    depth_buffer = np.zeros([h, w], dtype = np.float32, order = 'C') - 999999.

    vertices = vertices.astype(np.float32).copy()
    triangles = triangles.astype(np.int32).copy()
    colors = colors.astype(np.float32).copy()

    mesh_render.render_colors_core(
            image, vertices, triangles,
            colors,
            depth_buffer,
            black_map,
            vertices.shape[0], triangles.shape[0], 
            h, w, c)        

    image = (image*255).astype(np.uint8)
    black_map = (black_map*255).astype(np.uint8)

    return image, black_map



def main():
    
    fdir = os.path.join(os.path.dirname(__file__),'../datas/')
    filename = '0.obj'
    obj = OBJ(fdir, filename)

    vertices = np.array(obj.vertices)
    colors = np.array(obj.v_colors)        
    face_ori = np.array(obj.faces)

    triangles = []
    for face in face_ori:
        face_idxs = face[0]
        triangles.append(face_idxs)
    triangles = np.array(triangles)

    # print(vertices.shape)
    # print(colors.shape)
    # print(triangles.shape)

    h, w, c = 400,500,3
    bgr = np.zeros((h,w,c), dtype= np.float32)
    img_render,_ = render_colors(vertices, triangles, colors, bgr)
    img_render = cv2.flip(img_render,0)
    cv2.imshow('render result', img_render)
    cv2.waitKey(0)

if __name__ == '__main__':
    main()