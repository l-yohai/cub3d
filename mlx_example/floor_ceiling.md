# floor and ceiling

## Introduction

이전의 Raycasting 문서에서 텍스쳐가 없는 평평한 벽(Untextured Raycasting)과 텍스쳐가 있는 벽(Textured Raycasting)을 어떻게 렌더링했는지 살펴보았습니다. 이번시간에는 바닥과 천장(Floor and Ceiling)을 렌더링하는 방법을 살펴볼 것입니다. 만약 바닥과 천장을 텍스쳐 없이 구현하고 싶으시다면, 이후 추가로 나와있는 코드를 살펴볼 필요는 없습니다. 더 많은 계산이 필요하기 때문이죠.

Wolfenstein 3D(볼펜슈타인3d by 독일어)의 바닥과 천장은 텍스쳐가 없지만, 다른 볼펜슈타인 이후 등장한 레이캐스팅이 사용된 게임들은 바닥과 천장의 텍스쳐를 표현했죠.

전체코드보기(https://lodev.org/cgtutor/files/raycaster_floor.cpp)
<br>
<br>

## How it works

이전시간에서 벽은 수직으로 그렸었던 것과 달리, 바닥과 천장의 텍스쳐는 수평으로 그려야 합니다. 따라서 벽과 같은 방식, 수직선을 그리는 방법과는 “당.연.히” 다른 방법으로 그려야 합니다. 즉, ‘수평선’을 이용하는 것이죠. 벽을 바라보는 시야를 90도 회전시키면 수평선이 되겠죠? 재밌는 사실은, 벽을 그릴 때는 수직선의 텍스쳐를 하나만 사용했었던 반면 (벽 하나당 텍스쳐 하나) 바닥과 천장을 그릴 때는 여러 텍스쳐를 교차시킬 수 있죠. (아래 사진과 같이 두 텍스쳐를 교차로 그려낼수도, 한 텍스쳐가 다른 텍스쳐를 가로지르는 대각선을 그려낼 수도 있습니다.)

![image](https://user-images.githubusercontent.com/49181231/86937477-4372f680-c17a-11ea-8625-2dd49bee002d.png)
![image](https://user-images.githubusercontent.com/49181231/86937488-44a42380-c17a-11ea-95b2-ae3702db8f18.png)

천장을 그리는 것은 바닥을 그리는 것과 거의 동일하니까, 바닥 그리는 것만 설명할게요.

The floor casting(쉽게 바닥캐스팅이라고 하겠습니다.)
바닥캐스팅은 벽을 그리기 전에 이루어져야 합니다. 따라서 전체적인 로직은, 처음에 바닥과 천장을 다 그린 이후에 벽에 해당하는 픽셀들만 벽으로 덮어씌우는 방식입니다.
바닥캐스팅은 ‘주사선’의 방식, 즉 광선이죠, 으로 이루어집니다. (work scanline by scanline) 일반적으로 TV가 한 화면을 만들 때 직선방향으로 수많은 직선레이저를 쏘는데, 이 레이저들은 하나하나의 라인으로 이루어져 있고, 이 라인 하나를 주사선이라고 부릅니다. (아래 그림의 색깔 별 라인이라고 생각하시면 될 것 같습니다.) 쉽게말하자면, 사람이 책을 위에서 아래로 읽어내는 것처럼 우리는 책을 읽을 때 ‘시야’를 위에서 아래로 흘려보내죠. 이것과 같은 방식으로 TV도 주사선이라는 ‘시야’를 직선방향으로 보내는 움직임을 통해 한 장의 화면을 만드는 것이죠. 

![image](https://user-images.githubusercontent.com/49181231/86937512-4c63c800-c17a-11ea-9299-b44560062685.png)
[사진출처](https://www.google.com/url?sa=i&url=https%3A%2F%2Fko.wikipedia.org%2Fwiki%2F%25EC%2595%2584%25EB%2582%25A0%25EB%25A1%259C%25EA%25B7%25B8_%25ED%2585%2594%25EB%25A0%2588%25EB%25B9%2584%25EC%25A0%2584&psig=AOvVaw3ZxZ75Ta1zProX19XUqBD9&ust=1593704144066000&source=images&cd=vfe&ved=0CAIQjRxqFwoTCMDy3N6wrOoCFQAAAAAdAAAAABAD)

현재 스캔라인을 구하기 위해서는, 라인의 왼쪽 픽셀에 해당하는 바닥의 위치와 라인의 오른쪽 픽셀에 해당하는 바닥의 위치를 계산해야 합니다. 즉, 카메라로부터 쏘아올려진 작은 광선이 카메라 평면의 픽셀을 통과하여 바닥(floor)에 부딪히는 것을 계산해야 한다는 뜻입니다. (아직 포기하지 마세요, 저도 문과입니다.) 이를 위한 공식은, 바닥캐스팅 코드에서 자세하게 설명하겠습니다.

스캔라인은 측면 끝이 아닌, 그 사이를 향해 직선적으로 뻗어나가며, 결국에 바닥 또는 천장과 부딪히게 되는 지점을 통해서 스캔라인에 해당하는 픽셀, 즉 바닥과 부딪힌 광선의 좌표를 얻을 수 있습니다. 왜냐하면 위에서 말했듯이 벽과는 달리 바닥과 천장 텍스쳐는 완벽하게 수평이기 때문이죠. 만약에 바닥이나 천장이 비스듬하다면, 우리는 더 어려운 원근법을 사용하여 정확한 텍스쳐를 매핑해야 할 필요가 있을거에요. 그러니까 비스듬한 경우는 생각하지 말자구요. (찡긋)

참고로 Ádám Tóth라는 사람은 2019년에 수평인 스캔라인 기법에 대해 아이디어와 코드를 제공해주셨습니다. 앞서 살펴본 수직기반 기법보다 수평기법이 훨씬 빠르고 (mlx_pixel_put대신 mlx_put_image_to_window를 쓰는 이유가 이것입니다.) 레이캐스팅 게임이 실제 동작하는 방식과 일치합니다. 뒤에서 더 자세하게 다룰거에요.
<br>
<br>

## The Code

코드에서는 볼펜슈타인 텍스쳐를 로드할 거에요. 다운로드 링크https://lodev.org/cgtutor/files/wolftex.zip

만약 다른 텍스쳐를 사용하고 싶으면, 텍스쳐를 생성했던 파트의 코드를 사용하세요. 그러나 이것보단 안멋있을거에요.

코드의 첫 번째 파트는 앞의 레이캐스팅 튜토리얼 코드와 완벽하게 일치하지만, 새로운 부분이 있어요. 바로바로바로, 새로운 맵이죠. 이번 코드에서는 새로운 변수가 사용되고, 텍스쳐를 이미지로 로드하고, 벽을 수직선으로 그릴거에요. 텍스쳐를 로딩하기 위해서는 앞의 코드를 해결하고 오세요!

``` C
#define screenWidth 640
#define screenHeight 480
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24

int worldMap[mapWidth][mapHeight]=
{
  {8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4},
  {8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
  {8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6},
  {8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
  {8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
  {8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6},
  {8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6},
  {7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6},
  {7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6},
  {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4},
  {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6},
  {7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6},
  {7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3},
  {2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3},
  {2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
  {2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
  {1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3},
  {2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5},
  {2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
  {2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
  {2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
  {2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
  {2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5}
};

Uint32 buffer[screenHeight][screenWidth]; // y-coordinate first because it works per scanline

int main(int /*argc*/, char */*argv*/[])
{
  double posX = 22.0, posY = 11.5;  //x and y start position
  double dirX = -1.0, dirY = 0.0; //initial direction vector
  double planeX = 0.0, planeY = 0.66; //the 2d raycaster version of camera plane

  double time = 0; //time of current frame
  double oldTime = 0; //time of previous frame

  std::vector<Uint32> texture[8];
  for(int i = 0; i l-< 8; i++) texture[i].resize(texWidth * texHeight);

  screen(screenWidth,screenHeight, 0, "Raycaster");

  //load some textures
  unsigned long tw, th, error = 0;
  error |= loadImage(texture[0], tw, th, "pics/eagle.png");
  error |= loadImage(texture[1], tw, th, "pics/redbrick.png");
  error |= loadImage(texture[2], tw, th, "pics/purplestone.png");
  error |= loadImage(texture[3], tw, th, "pics/greystone.png");
  error |= loadImage(texture[4], tw, th, "pics/bluestone.png");
  error |= loadImage(texture[5], tw, th, "pics/mossy.png");
  error |= loadImage(texture[6], tw, th, "pics/wood.png");
  error |= loadImage(texture[7], tw, th, "pics/colorstone.png");
  if(error) { std::cout << "error loading images" << std::endl; return 1; }

  //start the main loop
  while(!done())
  {
```

자, 메인루프를 만들어줄 차례입니다. 바닥캐스팅을 위해서 수직선 대신 수평선을 이용한다는 것 잊지 마세요. 이 공식에서는 <현재 행의 카메라에서부터 바닥까지의 수평거리(horizontal distance)인 열의 거리(rowDistance)> 변수가 사용되고 이 값은 화면 중앙에서 현재 픽셀까지의 거리 p를 z의 위치인 posZ에서 나누어준 값입니다. 

카메라의 광선은 특정 높이(posZ)에 있는 자신을 거쳐가며 (화면 픽셀을 포함하여 그려진 수직평면을 통해) 카메라로부터 수평거리가 1인 카메라의 앞지점, 즉 (posZ – p)보다 낮은 수직위치 p를 통과합니다. 이 지점을 지날 때 광선은 p단위로 수직이동하며, 1단위로 수평이동합니다. 바닥에 선이 닿기 위해서는 posZ단위로 이동해야 합니다. 이것은 (광선 이동 시) 같은 비율을 가지고 수평으로 이동해야 한다는 말과도 같습니다. 카메라 평면을 통과하는 비율이 1 / p 이므로, 바닥에 닿기 위해서는 posZ번 만큼만 더 이동하면 되는데, 즉 posZ * (1 / p) = posZ / p라는 것을 알 수 있죠.

참고: 이 단계적인 작업은 텍스쳐를 맵핑(mapping)하는 단계입니다. 이 맵핑은 한 픽셀을 여러 각도로 계산하는 것이 아닌 두 점을 이용한 직선으로 계산한다는 것인데, 원근법적 이유로 정확한 계산법은 아니지만, 저희가 구현해야할 것은 바닥과 천장이 완벽하게 수평이고 벽에 완벽하게 수직이기 때문에 이용할 수 있는 계산이에요.

``` C
    //FLOOR CASTING
    for(int y = 0; y < h; y++)
    {
      // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
      float rayDirX0 = dirX - planeX;
      float rayDirY0 = dirY - planeY;
      float rayDirX1 = dirX + planeX;
      float rayDirY1 = dirY + planeY;

      // Current y position compared to the center of the screen (the horizon)
      int p = y - screenHeight / 2;

      // Vertical position of the camera.
      float posZ = 0.5 * screenHeight;

      // Horizontal distance from the camera to the floor for the current row.
      // 0.5 is the z position exactly in the middle between floor and ceiling.
      float rowDistance = posZ / p;

      // calculate the real world step vector we have to add for each x (parallel to camera plane)
      // adding step by step avoids multiplications with a weight in the inner loop
      float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenWidth;
      float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenWidth;

      // real world coordinates of the leftmost column. This will be updated as we step to the right.
      float floorX = posX + rowDistance * rayDirX0;
      float floorY = posY + rowDistance * rayDirY0;

      for(int x = 0; x < screenWidth; ++x)
      {
        // the cell coord is simply got from the integer parts of floorX and floorY
        int cellX = (int)(floorX);
        int cellY = (int)(floorY);

        // get the texture coordinate from the fractional part
        int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
        int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

        floorX += floorStepX;
        floorY += floorStepY;

        // choose texture and draw the pixel
        int floorTexture = 3;
        int ceilingTexture = 6;
        Uint32 color;

        // floor
        color = texture[floorTexture][texWidth * ty + tx];
        color = (color >> 1) & 8355711; // make a bit darker
        buffer[y][x] = color;

        //ceiling (symmetrical, at screenHeight - y - 1 instead of y)
        color = texture[ceilingTexture][texWidth * ty + tx];
        color = (color >> 1) & 8355711; // make a bit darker
        buffer[screenHeight - y - 1][x] = color;
      }
    }
```

다음은 벽을 캐스팅하는 코드입니다. 이전 튜토리얼과 완전히 동일하고, 새로 나타나는 것은 없습니다.

벽 캐스팅은 바닥캐스팅 직후에 시작합니다. 바닥과 천장을 캐스팅하는 것처럼 수평선이 아닌 수직선을 이용하기 때문입니다.

``` C
    //WALL CASTING
    for(int x = 0; x < w; x++)
    {
      //calculate ray position and direction
      double cameraX = 2 * x / double(w) - 1; //x-coordinate in camera space
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX;

      //which box of the map we're in
      int mapX = int(posX);
      int mapY = int(posY);

      //length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;

      //length of ray from one x or y-side to next x or y-side
      double deltaDistX = std::abs(1 / rayDirX);
      double deltaDistY = std::abs(1 / rayDirY);
      double perpWallDist;

      //what direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; //was there a wall hit?
      int side; //was a NS or a EW wall hit?

      //calculate step and initial sideDist
      if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }
      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }
      //perform DDA
      while (hit == 0)
      {
        //jump to next map square, OR in x-direction, OR in y-direction
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        //Check if ray has hit a wall
        if (worldMap[mapX][mapY] > 0) hit = 1;
      }

      //Calculate distance of perpendicular ray (Euclidean distance will give fisheye effect!)
      if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
      else           perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

      //Calculate height of line to draw on screen
      int lineHeight = (int)(h / perpWallDist);

      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + h / 2;
      if(drawStart < 0) drawStart = 0;
      int drawEnd = lineHeight / 2 + h / 2;
      if(drawEnd >= h) drawEnd = h - 1;
      //texturing calculations
      int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

      //calculate value of wallX
      double wallX; //where exactly the wall was hit
      if (side == 0) wallX = posY + perpWallDist * rayDirY;
      else           wallX = posX + perpWallDist * rayDirX;
      wallX -= floor((wallX));

      //x coordinate on the texture
      int texX = int(wallX * double(texWidth));
      if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
      if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

      // How much to increase the texture coordinate per screen pixel
      double step = 1.0 * texHeight / lineHeight;
      // Starting texture coordinate
      double texPos = (drawStart - h / 2 + lineHeight / 2) * step;
      for(int y = drawStart; y<drawEnd; y++)
      {
        // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
        int texY = (int)texPos & (texHeight - 1);
        texPos += step;
        Uint32 color = texture[texNum][texWidth * texY + texX];
        //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        if(side == 1) color = (color >> 1) & 8355711;
        buffer[y][x] = color;
      }
```

마지막으로 스크린이 다 그려졌으면, 지우고나서 키를 핸들링합니다. 이 코드도 이전과 똑같아요.
``` C
    drawBuffer(buffer[0]);
    for(int y = 0; y < h; y++) for(int x = 0; x < w; x++) buffer[y][x] = 0; //clear the buffer instead of cls()

    //timing for input and FPS counter
    oldTime = time;
    time = getTicks();
    double frameTime = (time - oldTime) / 1000.0; //frametime is the time this frame has taken, in seconds
    print(1.0 / frameTime); //FPS counter
    redraw();

    //speed modifiers
    double moveSpeed = frameTime * 3.0; //the constant value is in squares/second
    double rotSpeed = frameTime * 2.0; //the constant value is in radians/second
    readKeys();
    //move forward if no wall in front of you
    if (keyDown(SDLK_UP))
    {
      if(worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false) posX += dirX * moveSpeed;
      if(worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
    }
    //move backwards if no wall behind you
    if (keyDown(SDLK_DOWN))
    {
      if(worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
      if(worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
    }
    //rotate to the right
    if (keyDown(SDLK_RIGHT))
    {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
      dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
      planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    //rotate to the left
    if (keyDown(SDLK_LEFT))
    {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
  }
}
```

이 코드를 실행하면 이제 이렇게 보일겁니다.

![image](https://user-images.githubusercontent.com/49181231/86939065-145d8480-c17c-11ea-9d65-82edc921c46d.png)

여기서 만든 벽과 바닥 레이캐스팅 코드는, 고해상도에서 매우 느리고 최적화의 여지가 남아있어요.
<br>
<br>

### Special Tricks

이 트릭은 사실 특별한 것은 아닙니다.

바닥과 천장 텍스쳐의 크기를 조정하려면 (예를들어 무늬를 4배 더 확대하겠다!) 이 부분을 수정하시면 됩니다.

``` C
        int floorTexX, floorTexY;
        floorTexX = int(currentFloorX * texWidth) % texWidth;
        floorTexY = int(currentFloorY * texHeight) % texHeight;
```
이것을
``` C
        int floorTexX, floorTexY;
        floorTexX = int(currentFloorX * texWidth / 4) % texWidth;
        floorTexY = int(currentFloorY * texHeight / 4) % texHeight;
```
이렇게.

![image](https://user-images.githubusercontent.com/49181231/86939300-5ab2e380-c17c-11ea-8c68-9ff0adb12bf5.png)

짜잔.

바닥의 무늬를 하나의 이미지 말고 두 개의 이미지를 사용하여 그릴수도 있습니다.
본 코드에서 벽이 아닌 부분(돌아다닐 수 있는 부분)은 맵에서의 0값을 가지고 있는데, 이것은 바닥의 텍스쳐로 사용될 수 없는 값입니다. 따라서 벽이 아닌 부분을 음수 등으로 만들면, 음수는 벽이 아니기 때문에 다른 바닥의 질감을 나타내는 값으로서 사용할 수 있습니다. 천장도 마찬가지로 할 수 있으며, 본 코드에서 사용된 맵과 다른 맵을 이용하는 것도 고려해볼 수 있겠죠.

지도상의 x좌표와 y좌표의 합이 짝수라면 텍스쳐 3번을, 홀수라면 텍스쳐 4번을 얻는 방식으로 체크무늬 패턴을 바닥에 그려보겠습니다.

현재 맵의 좌표를 얻고 싶으면 currentFloorX와 currentFloorY가 나타나는 부분을, 아래와 같이 변경하면 됩니다.


``` C
      //draw the floor from drawEnd to the bottom of the screen
      for(int y = drawEnd + 1; y < h; y++)
      {
        currentDist = h / (2.0 * y - h); //you could make a small lookup table for this instead

        double weight = (currentDist - distPlayer) / (distWall - distPlayer);

        double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
        double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

        int floorTexX, floorTexY;
        floorTexX = int(currentFloorX * texWidth) % texWidth;
        floorTexY = int(currentFloorY * texHeight) % texHeight;

        int checkerBoardPattern = (int(currentFloorX) + int(currentFloorY))) % 2;
        int floorTexture;
        if(checkerBoardPattern == 0) floorTexture = 3;
        else floorTexture = 4;

        //floor
        buffer[y][x] = (texture[floorTexture][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
        //ceiling (symmetrical!)
        buffer[h - y][x] = texture[6][texWidth * floorTexY + floorTexX];
      }
    }
```

비슷한 방법으로 각 타일의 바닥 질감을 다르게 선택할 수도 있겠죠. currentFloorX의 정수 부분은 현재 맵에서 바닥질감을 선택하는 좌표로 사용되고, 실수부분은 텍스쳐에 texel 좌표로 사용됩니다.

![image](https://user-images.githubusercontent.com/49181231/86940179-74a0f600-c17d-11ea-86fe-2b5c8c74d32a.png)


체크무늬 패턴부분 코드를 "(int(currentFloorX) + int(currentFloorY)) % 2" into "(int(currentFloorX + currentFloorY)) % 2" 이렇게 수정하면, 체크무늬가 아니라 대각선 무늬를 표현할 수 있습니다.

![image](https://user-images.githubusercontent.com/49181231/86940204-7b2f6d80-c17d-11ea-89e3-8ee1a137938a.png)
