# sprite

## introduction

앞서 다루었던 레이캐스팅에서는 무엇인가가 빠져있어요. 그것은 바로, '아이템'입니다. 이것을 표현하기 위해서는 이미지가 필요하죠.

전체코드보기(https://lodev.org/cgtutor/raycasting3.html)
<br>
<br>

## How it works

Sprite를 그릴 때 사용하는 기술은 레이캐스팅과는 전적으로 다릅니다. 대신 3D 엔진에서 sprite를 그리는 방식과 매우 유사하게 작동합니다. 2d 이미지를 그려넣어야 하기 때문에, 레이캐스팅과 결합시키기 위한 몇 가지 기법이 사용될 것입니다.

스프라이트를 그리는 것은 벽과 바닥을 그린 이후에 합니다.


전체적인 로직은 아래와 같습니다.
```
1. 벽을 레이캐스팅하는 동안 각 수직선의 거리를 Z_BUFFER에 담습니다.
2. 플레이어와 스프라이트의 거리를 계산합니다.
3. 가장멀리 있는 스프라이트부터 카메라벡터와 가장 가까운 스프라이트까지, 거리를 이용하여 정렬합니다.
4. 카메라 평면에 스프라이트를 비춥니다. 즉, 스프라이트의 위치에서 플레이어의 위치를 뺀 다음에, 그것을 카메라의 (2 x 2)크기 역행렬과 곱합니다.
5. 수직거리(perpWallDist)를 이용하여 화면상의 스프라이트 크기(x와 y방향 모두)를 계산합니다.
6. 스프라이트를 수직으로 그리며, Z_BUFFER보다 거리가 멀어지면 그리는 것을 멈춥니다.
7. 수직선을 픽셀별로 그리고, 안보이는 색상이 있는지, 모든 스프라이트가 직사각형인지 확인해야 합니다.
```
* 수직선을 그릴 때 Z_BUFFER를 업데이트할 필요는 없습니다. 스프라이트는 정렬되어 있기 때문에, 멀리있는 것부터 그리고, 가까운 건 마지막에 그려지기 때문입니다.
* 화면에 스프라이트를 그려넣는 방법은 3d 렌더링 수학이론에서 자세하게 설명되기에, 이 튜토리얼에서는 설명하지 않습니다. 여기는 2d로 이루어져 있고 화려한 카메라 기법이 사용되지 않기 때문입니다. 
* 스프라이트의 좌표를 카메라 공간에 가져오기 위해서는, 스프라이트의 위치에서 플레이어의 위치를 뺀 값, 즉 플레이어에 대한 상대적인 위치값을 가지고 있어야 합니다. 
* 위치값을 얻은 이후에는 스프라이트의 방향을 플레이어에 맞게 회전시켜야 합니다. 카메라는 비스듬히 움직일 수 있고, 특정한 사이즈를 가지고 있기 때문에, 실제로는 회전이 아니라 변형(transformation)입니다. 이 변형은 플레이어에 대한 스프라이트의 상대적위치와 카메라의 역행렬과 곱하여 이루어집니다. 

카메라 행렬
```
[planeX   dirX]
[planeY   dirY]
```
카메라의 역행렬
```
____________1___________    [dirY      -dirX]
(planeX*dirY-dirX*planeY) * [-planeY  planeX]
```
이후에 카메라 위치에서 스프라이트의 (x, y)좌표를 얻을 수 있습니다. 여기에서 y좌표는 화면의 내부 깊이(z)를 의미합니다. 스크린에 스프라이트를 그리기 위해서는, x를 z로 나눈 이후에 픽셀 좌표로 변환 이후 크기를 조정해야 합니다.

이것처럼 sprite들을 배치할 때는 많은 일들을 해야 합니다. 각각의 물체는 부동소수점의 좌표를 가질 수 있고, 바닥 중앙에 정확히 있을 필요가 없기 때문에 스프라이트 객체의 목록(본 코드에서는 벡터를 사용하지만, 2차원 배열로 표현가능합니다.)을 만들어서 좌표와 텍스쳐를 미리 지정할 수 있습니다.

## The code

대체로 이전 튜토리얼의 코드와 비슷하지만 추가된 부분들이 군데군데 있습니다.

스프라이트 캐스팅에서는 새로운 구조체, numSprite 변수, 스프라이트 배열 등이 새롭게 등장하고, 버블정렬이 사용됩니다.

```C
#define screenWidth 640
#define screenHeight 480
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24

int worldMap[mapWidth][mapHeight] =
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

struct Sprite
{
  double x;
  double y;
  int texture;
};

#define numSprites 19

Sprite sprite[numSprites] =
{
  {20.5, 11.5, 10}, //green light in front of playerstart
  //green lights in every room
  {18.5,4.5, 10},
  {10.0,4.5, 10},
  {10.0,12.5,10},
  {3.5, 6.5, 10},
  {3.5, 20.5,10},
  {3.5, 14.5,10},
  {14.5,20.5,10},

  //row of pillars in front of wall: fisheye test
  {18.5, 10.5, 9},
  {18.5, 11.5, 9},
  {18.5, 12.5, 9},

  //some barrels around the map
  {21.5, 1.5, 8},
  {15.5, 1.5, 8},
  {16.0, 1.8, 8},
  {16.2, 1.2, 8},
  {3.5,  2.5, 8},
  {9.5, 15.5, 8},
  {10.0, 15.1,8},
  {10.5, 15.8,8},
};

Uint32 buffer[screenHeight][screenWidth]; // y-coordinate first because it works per scanline

//1D Zbuffer
double ZBuffer[screenWidth];

//arrays used to sort the sprites
int spriteOrder[numSprites];
double spriteDistance[numSprites];

//function used to sort the sprites
void sortSprites(int* order, double* dist, int amount);

int main(int /*argc*/, char */*argv*/[])
{
  double posX = 22.0, posY = 11.5; //x and y start position
  double dirX = -1.0, dirY = 0.0; //initial direction vector
  double planeX = 0.0, planeY = 0.66; //the 2d raycaster version of camera plane

  double time = 0; //time of current frame
  double oldTime = 0; //time of previous frame

  std::vector<Uint32> texture[11];
  for(int i = 0; i < 11; i++) texture[i].resize(texWidth * texHeight);
```

세 개의 새로운 텍스쳐가 로딩되는데, 이것이 바로 스프라이트입니다.
```C
  screen(screenWidth, screenHeight, 0, "Raycaster");

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

  //load some sprite textures
  error |= loadImage(texture[8], tw, th, "pics/barrel.png");
  error |= loadImage(texture[9], tw, th, "pics/pillar.png");
  error |= loadImage(texture[10], tw, th, "pics/greenlight.png");
  if(error) { std::cout << "error loading images" << std::endl; return 1; }
```

새로운 메인루프가 등장합니다.
```C
  //start the main loop
  while(!done())
  {
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

    // WALL CASTING
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
        int color = texture[texNum][texWidth * texY + texX];
        //make color darker for y-sides: R, G and B byte each divided through two with a 'shift' and an 'and'
        if(side == 1) color = (color >> 1) & 8355711;
        buffer[y][x] = color;
      }
```

벽 캐스팅 이후에, ZBuffer를 세팅해주어야 합니다. 이것은 모든 픽셀과의 거리가 아니라 오직 벽과의 수직선 거리만을 포함시키기 때문에 1차원 배열로 이루어져 있습니다. 따라서, 루프가 끝나기 직전에 ZBuffer를 세팅해 주면 됩니다.

```C
      //SET THE ZBUFFER FOR THE SPRITE CASTING
      ZBuffer[x] = perpWallDist; //perpendicular distance is used
    }
```

벽과 바닥 색상이 버퍼에 담기고 난 이후에 스프라이트가 그려집니다. 이 코드는 최적화가 되어있지 않습니다. 몇 가지의 개선사항은 나중에 설명하겠습니다. 먼저 스프라이트를 위에서 설명한 대로 거리순으로 정렬하고 가장 멀리 있는 스프라이트가 먼저 그려지게 합니다. 이후, 각각의 스프라이트의 크기를 계산한 다음 화면에 그려냅니다.

다른 레이캐스팅처럼 각도와 위치로 표현하는 것이 아닌 카메라행렬을 사용한다는 것은 굉장히 유용하죠.

* 수직거리가 사용되기 때문에 스프라이트의 정렬을 위해 계산된 거리는 사용되지 않습니다.
* 스프라이트를 정렬할 때는 거리의 제곱근을 하지 않아도 됩니다. 따라서 이 계산을 위해 시간을 낭비하지 맙시다!

```C
    //SPRITE CASTING
    //sort sprites from far to close
    for(int i = 0; i < numSprites; i++)
    {
      spriteOrder[i] = i;
      spriteDistance[i] = ((posX - sprite[i].x) * (posX - sprite[i].x) + (posY - sprite[i].y) * (posY - sprite[i].y)); //sqrt not taken, unneeded
    }
    sortSprites(spriteOrder, spriteDistance, numSprites);

    //after sorting the sprites, do the projection and draw them
    for(int i = 0; i < numSprites; i++)
    {
      //translate sprite position to relative to camera
      double spriteX = sprite[spriteOrder[i]].x - posX;
      double spriteY = sprite[spriteOrder[i]].y - posY;

      //transform sprite with the inverse camera matrix
      // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
      // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
      // [ planeY   dirY ]                                          [ -planeY  planeX ]

      double invDet = 1.0 / (planeX * dirY - dirX * planeY); //required for correct matrix multiplication

      double transformX = invDet * (dirY * spriteX - dirX * spriteY);
      double transformY = invDet * (-planeY * spriteX + planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D

      int spriteScreenX = int((w / 2) * (1 + transformX / transformY));

      //calculate height of the sprite on screen
      int spriteHeight = abs(int(h / (transformY))); //using 'transformY' instead of the real distance prevents fisheye
      //calculate lowest and highest pixel to fill in current stripe
      int drawStartY = -spriteHeight / 2 + h / 2;
      if(drawStartY < 0) drawStartY = 0;
      int drawEndY = spriteHeight / 2 + h / 2;
      if(drawEndY >= h) drawEndY = h - 1;

      //calculate width of the sprite
      int spriteWidth = abs( int (h / (transformY)));
      int drawStartX = -spriteWidth / 2 + spriteScreenX;
      if(drawStartX < 0) drawStartX = 0;
      int drawEndX = spriteWidth / 2 + spriteScreenX;
      if(drawEndX >= w) drawEndX = w - 1;

      //loop through every vertical stripe of the sprite on screen
      for(int stripe = drawStartX; stripe < drawEndX; stripe++)
      {
        int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
        //the conditions in the if are:
        //1) it's in front of camera plane so you don't see things behind you
        //2) it's on the screen (left)
        //3) it's on the screen (right)
        //4) ZBuffer, with perpendicular distance
        if(transformY > 0 && stripe > 0 && stripe < w && transformY < ZBuffer[stripe])
        for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
        {
          int d = (y) * 256 - h * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
          int texY = ((d * texHeight) / spriteHeight) / 256;
          Uint32 color = texture[sprite[spriteOrder[i]].texture][texWidth * texY + texX]; //get current color from the texture
          if((color & 0x00FFFFFF) != 0) buffer[y][stripe] = color; //paint pixel if it isn't black, black is the invisible color
        }
      }
    }
```

모든것이 그려지고 난 이후에 스크린을 업데이트하고 키를 핸들링합니다.

```C
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

![image](https://user-images.githubusercontent.com/49181231/86943602-9bf9c200-c181-11ea-841b-58fa12bbf79f.png)
![image](https://user-images.githubusercontent.com/49181231/86943609-9e5c1c00-c181-11ea-8377-a15e26c11e41.png)

* green light는 매우 작은 스프라이트이지만, 프로그램은 보이지 않는 모든 픽셀을 통과하여 색을 확인합니다. 위치상 눈에 보이지 않는 스프라이트를 확인하고, 눈에 보이는 픽셀을 포함하는 작은 직사각형을 그리게 되면 더 빨리 만들 수 있습니다.

* 일부 스프라이트를 통과하지 못하게 하려면, 충돌 감지를 위해 이동 시 모든 스프라이트에 대한 플레이어와의 거리를 확인해야 합니다.
<br>
<br>

## Scaling Sprites

스프라이트를 더 크게 혹은 더 작게 만들거나 스프라이트의 위치를 위 아래로 이동시키는 것은 생각보다 쉽습니다.

스프라이트를 축소하기 위해서는 spriteWidth와 spriteHeight를 무언가로 나누기 하면 됩니다. 예를들어 스프라이트의 높이(spriteHeight)를 절반으로 줄이게 되면 바닥에서 위로 붕 떠 있는 것처럼 보이기 때문에 스프라이트를 축소하기 위한 변수 uDiv, vDiv, vMove와 vMoveScreen이 추가됩니다.

```C
      //parameters for scaling and moving the sprites
      #define uDiv 1
      #define vDiv 1
      #define vMove 0.0
      int vMoveScreen = int(vMove / transformY);

      //calculate height of the sprite on screen
      int spriteHeight = abs(int(h / (transformY))) / vDiv; //using 'transformY' instead of the real distance prevents fisheye
      //calculate lowest and highest pixel to fill in current stripe
      int drawStartY = -spriteHeight / 2 + h / 2 + vMoveScreen;
      if(drawStartY < 0) drawStartY = 0;
      int drawEndY = spriteHeight / 2 + h / 2 + vMoveScreen;
      if(drawEndY >= h) drawEndY = h - 1;

      //calculate width of the sprite
      int spriteWidth = abs( int (h / (transformY))) / uDiv;
      int drawStartX = -spriteWidth / 2 + spriteScreenX;
      if(drawStartX < 0) drawStartX = 0;
      int drawEndX = spriteWidth / 2 + spriteScreenX;
      if(drawEndX >= w) drawEndX = w - 1;


      //loop through every vertical stripe of the sprite on screen
      for(int stripe = drawStartX; stripe < drawEndX; stripe++)
      {
        int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
        //the conditions in the if are:
        //1) it's in front of camera plane so you don't see things behind you
        //2) it's on the screen (left)
        //3) it's on the screen (right)
        //4) ZBuffer, with perpendicular distance
        if(transformY > 0 && stripe > 0 && stripe < w && transformY < ZBuffer[stripe])
        for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
        {
          int d = (y-vMoveScreen) * 256 - h * 128 + spriteHeight * 128;  //256 and 128 factors to avoid floats
          int texY = ((d * texHeight) / spriteHeight) / 256;
          Uint32 color = texture[sprite[spriteOrder[i]].texture][texWidth * texY + texX]; //get current color from the texture
          if((color & 0x00FFFFFF) != 0) buffer[y][stripe] = color; //paint pixel if it isn't black, black is the invisible color
        }
      }
    }
```

uDiv = 2, vDiv = 2, vMove = 0.0 일 때 스프라이트

![image](https://user-images.githubusercontent.com/49181231/86944530-d7e15700-c182-11ea-8331-d77791edd4a3.png)

vMove가 64.0일 때

![image](https://user-images.githubusercontent.com/49181231/86944548-de6fce80-c182-11ea-962d-4492afed7302.png)

스프라이트가 바닥에 붙어있기 위해 vMove를 더 크게 만든다면, 땅 속에 박혀있는 것처럼 보일거에요. ZBuffer는 일차원 배열이고, 스프라이트가 벽 앞이나 뒤에 있을 때만 감지할 수 있기 때문이죠.

![image](https://user-images.githubusercontent.com/49181231/86944569-e4fe4600-c182-11ea-8d07-8a5f7996bde3.png)

물론 통의 크기를 줄임으로써, 그린라이트도 낮아져서 천장에서 떨어지게 됐어요. 이를 해결하기 위해서는 고유한 uDiv, vDiv, vMove 변수를 지정해야 하는데, 한가지 힌트는 스프라이트 구조체에 넣고 같이 사용하는 거에요.

![image](https://user-images.githubusercontent.com/49181231/86944573-e7f93680-c182-11ea-97f6-747574905edb.png)

<br>
<br>

## Translucent Sprites

우리는 RGB색상으로 이미지를 만들기 때문에 반투명한 스프라이트를 만드는 것은 매우 간단합니다. 
``` C
if((color & 0x00FFFFFF) != 0) buffer[y][stripe] = color; //paint pixel if it isn't black, black is the invisible color
```

이것을
``` C
if((color & 0x00FFFFFF) != 0) buffer[y][stripe] = RGBtoINT(INTtoRGB(buffer[y][stripe]) / 2 + INTtoRGB(color) / 2); //paint pixel if it isn't black, black is the invisible color
```
이렇게 바꾸면 돼요.

![image](https://user-images.githubusercontent.com/49181231/86945009-7a013f00-c183-11ea-9737-f532f4c0f8f4.png)

더 반투명하게 만들고 싶으면
```C
if((color & 0x00FFFFFF) != 0) buffer[y][stripe] = RGBtoINT(3 * INTtoRGB(buffer[y][stripe]) / 4 + INTtoRGB(color) / 4); //paint pixel if it isn't black, black is the invisible color
```

이렇게 바꾸면 됩니다.

![image](https://user-images.githubusercontent.com/49181231/86945022-7cfc2f80-c183-11ea-9e32-0c4e9158ff8d.png)

더 많은 트릭들이 있을 수 있죠. 벽을 더 negative한 색으로 만들 수도 있어요.
```C
if((color & 0x00FFFFFF) != 0) buffer[y][stripe] = RGBtoINT((RGB_White - INTtoRGB(buffer[y][stripe])) / 2 + INTtoRGB(color) / 2); //paint pixel if it isn't black, black is the invisible color
```

![image](https://user-images.githubusercontent.com/49181231/86945035-808fb680-c183-11ea-9bda-e21b46f2cf31.png)

