import os



def main():

    order = [
        'Utils/Math.h',
        'Utils/Vertex.h',
        'Utils/FPSCounter.h',
        'Renderer/Console.h',
        'Renderer/Camera.h',
        'Renderer/DepthBuffer.h',
        'Renderer/Inputs.h',
        'Renderer/Shapes.h',
        'Renderer/Renderer.h'
        ]

    output_h = 'glascii.h'
    with open(output_h, 'w') as outfile:

        for file in order:
            with open(file) as f:
                trueFn = file.split('/')[1].split('.')[0]
                outfile.write("#ifndef " + trueFn.capitalize() + "_IMPL_H\n")
                outfile.write("#define " + trueFn.capitalize() + "_IMPL_H\n")

                Lines = f.readlines()
                for l in Lines:
                    if l.find("#include \"") == -1 and l.find("#pragma once") == -1:
                        outfile.write(l)

                outfile.write("\n#endif\n")



if __name__ == "__main__":
    main()