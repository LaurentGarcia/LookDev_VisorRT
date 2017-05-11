# lookdev_Viewer

LookDev Viewport,
=====
[![Build Status](https://github.com/LaurentGarcia/LookDev_VisorRT.git)](https://github.com/LaurentGarcia/LookDev_VisorRT)

(This application is free, intended to learn propose and giving the tools to the user for evaluate, compile and use their own shaders. E-mail: laurent.garcia.carro at gmail)

Lookdev viewport, is a OpenGL and c++ based application. The tool is able to load models format as .obj, fbx and all support formats than Assimp library supported. http://assimp.sourceforge.net/main_features_formats.html . In the future, I try to give support to load Alembic format too. The Viewport, give you the possibility to load any texture, creat lights, and in the coming month IBL lighting too. In this way the user probably a texture artist or TD shading artist could check the textures seeing in realtime and seeing how react under the different lights that user create.

Lookdev Viewport has been designed to as standalone application, supporting Linux right now and in the incoming future windows too, It's a toy to debug our textures and the intention is quite simple. Trying to keep the user interface simple, intuitive and quick to use for an artist.


Libraries/acknowledgment
------------------------

Lookdev viewport born because I needed to learn from the scratch how to build a standalone application in openGl, focused in the latest Physical Based Shading techniques. I found in this tutorial https://learnopengl.com/#!About from Joey de Vries the base of my inspiration and some code is coming from it. I can't express with enough words how it helped me to create this application.

For the user interface, I'm using a library called DearImgui, written in c++ and openGl, which it's fantastic to this kind of applications: https://github.com/ocornut/imgui

And for model loading and import into openGl: http://assimp.sourceforge.net/main_features_formats.html

Binaries/Demo
-------------

To be continued!... soon!
