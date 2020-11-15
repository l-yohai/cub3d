# cub3d 및 raycasting tutorial

---

***Warning***

본 예제 코드에서는 맵의 방향이 정상적으로 출력되지 않는 (동서남북이 뒤바뀌어 출력됨) 문제가 있습니다.

이 해답은 공부하시면서 찾아보시길 바랍니다! 절대 귀찮아서가 아닙니다...... 😂😭🤔

* 힌트 : 딱 하나만 바꿔주면 됩니다. 맵의 Color를 채우는 반복문과 이미지를 Drawing 하는 반복문을 비교해보세요!

---

### 20.11.16 Update
* [Sprite번역](https://github.com/l-yohai/cub3d/blob/master/mlx_example/sprite.md) 중 `스프라이트를 그리기 위해서는 z를 x로 나눈 이후에...` 를 `x를 z로 나눈 이후에 ...` 로 수정하였습니다.
* 번역오류를 찾아주신 eun-park님 감사합니다.

### 20.09.09 Update
* sprite_utils.c sort함수 -> sprite가 겹쳐있을 때 깜빡거리는 문제 수정
* 문제 찾아서 수정해주신 sungslee님 감사합니다.

### 20.08.23 Update
* sprite_utils.c 45라인에서 if 조건문 안의 비교구문이 같았던 부분 수정
* 문제를 찾아주신 hycho님 감사합니다.

### 20.07.27 Update
* 05_sprite_raycast.c 파일의 sort 함수 부분에서 오류가 나던 부분 수정
* 문제를 찾아서 해결해주신 hakang님 감사합니다.

### 20.07.21 Update
* macos 코드들에서 segmentation fault가 나오는 부분들 수정
* 일부 코드에서 s키가 작동하지 않았던 오류 수정
* esc키 눌렀을 시 종료시키는 코드 추가

<br>

### 안내

이 자료는 taelee님의 [mlx_example](https://github.com/taelee42/mlx_example) 자료와 365kim님의 [raycasting_tutorial](https://github.com/365kim/raycasting_tutorial) 자료를 참고하여 만들어졌습니다.

그리고 365kim님이 번역하신 이후의 부분들을 번역하였습니다.

본 내용을 읽기 전 위 두 자료를 먼저 참고하시는 걸 추천드립니다.

앞서 좋은 자료를 만들어주신 두 분께 감사의 인사를 전합니다.

정말 많은 도움을 받았습니다. 감사합니다.

Cub3d 과제를 하면서 많이 참고하시는 [lodev](https://lodev.org/cgtutor/raycasting.html)사이트의 예제코드는 C++과 SDL 모듈을 이용하여 만들어져 있습니다. 

저처럼 C++를 모르거나, 레이캐스팅에 대한 번역문을 아무리 읽어보아도 이해하기 힘든 분들을 위해서 위 사이트의 예제코드를 C와 minilibx를 이용하여 변환시킨 예제를 만들어 보았습니다.

보시면 아시다시피 norminette 규칙을 지키지 않았고, for문 사용, 변수명 역시 위 사이트를 그대로 갖다 붙인거라 깔끔한 코드와 예제는 아니지만, 이것만으로도 레이캐스팅과 mlx를 이해하는 데 많은 도움이 되실거라고 생각합니다.

<br>
<br>

---
### 번역보기

- [Floor_ceiling](https://github.com/yohan9612/cub3d/blob/master/mlx_example/floor_ceiling.md)
- [Sprite](https://github.com/yohan9612/cub3d/blob/master/mlx_example/sprite.md)
<br>

---
### 주요함수설명

* linux에서 사용을 원하시는 분은 https://github.com/ilkou/minilibx 링크를 참조하여 필요한 모듈을 다운로드 받으시길 바랍니다.
* mlx_linux, key_linux, Xdev, Xming을 모두 설치하신 이후 아래와 같이 변경해주시면 됩니다.
```C
#include "mlx/mlx.h"
-> #include "mlx_linux/mlx.h"

#include "key_macos.h"
-> #include "key_linux.h"

mlx_hook(info.win, X_EVENT_KEY_PRESS, 0, &key_press, &info);
-> mlx_hook(info.win, X_EVENT_KEY_PRESS, 1L << 0, &key_press, &info);
```

* 컴파일

macos
```
gcc ~.c -Lmlx -lmlx -framework OpenGL -framework Appkit
```

linux
```
gcc ~.c -Lmlx_linux -lmlx -lXext -lX11
```

<br>

#### [01_untextured_raycast](https://github.com/yohan9612/cub3d/blob/master/mlx_example/01_untextured_raycast.c)

``` C
void	verLine(t_info *info, int x, int y1, int y2, int color)
{
	int	y;

	y = y1;
	while (y <= y2)
	{
		mlx_pixel_put(info->mlx, info->win, x, y, color);
		y++;
	}
}
```
- untextured_raycasting에서는 SDL의 verLine()함수를 이용하여 픽셀별로 이미지를 그려냅니다.
- - mimilibx에서는 mlx_pixel_put() 함수를 이용합니다.
<br>

#### [02_textured_raycast](https://github.com/yohan9612/cub3d/blob/master/mlx_example/02_textured_raycast.c)

``` C
void	draw(t_info *info)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			info->img.data[y * width + x] = info->buf[y][x];
		}
	}
	mlx_put_image_to_window(info->mlx, info->win, info->img.img, 0, 0);
}
```
- textured_raycasting에서는 픽셀별로 이미지를 찍는 방식이 아니라 버퍼에 데이터를 담고 mlx_put_image_to_window() 함수를 이용하여 버퍼에 담긴 이미지를 한 번에 출력합니다.
<br>

#### [03_img_textured_raycast](https://github.com/yohan9612/cub3d/blob/master/mlx_example/03_img_textured_raycast.c)

``` C
int	*load_image(t_info *info, char *path, t_img *img)
{
	int		*res;

	img->img = mlx_xpm_file_to_image(info->mlx, path, &img->img_width, &img->img_height);
	img->data = (int *)mlx_get_data_addr(img->img, &img->bpp, &img->size_l, &img->endian);

	res = (int *)malloc(sizeof(int) * (img->img_width * img->img_height));
	for (int y = 0; y < img->img_height; y++)
	{
		for (int x = 0; x < img->img_width; x++)
		{
			res[img->img_width * y + x] = img->data[img->img_width * y + x];
		}
	}
	mlx_destroy_image(info->mlx, img->img);
	return (res);
}
```
- img_textured_raycasting에서는 벽의 질감표현을 위해 xpm 이미지를 로드합니다. 이 때 mlx_xpm_file_to_image() 함수와 mlx_get_data_addr()함수를 이용하여 이미지를 받아온 이후, 여러 이미지를 받아와야 하기 때문에 mlx_destory_image()함수를 이용하여 이미지 데이터를 제거해줍니다.
<br>

#### [04_floor_ceiling_raycast](https://github.com/yohan9612/cub3d/blob/master/mlx_example/04_floor_ceiling.c)

``` C
for(int x = 0; x < width; ++x)
{
	...
	...

	// floor
	color = info->texture[floorTexture][texWidth * ty + tx];
	color = (color >> 1) & 8355711; // make a bit darker
	info->buf[y][x] = color;
	//ceiling (symmetrical, at screenHeight - y - 1 instead of y)
	color = info->texture[ceilingTexture][texWidth * ty + tx];
	color = (color >> 1) & 8355711; // make a bit darker
	info->buf[height - y - 1][x] = color;
}
```

- floor_ceiling 부분에서는 대부분의 로직이 이전 코드와 동일하지만, 벽은 수직으로 캐스팅을 시켰다면, 천장과 바닥은 수평으로 캐스팅을 시키는데, 이 부분이 추가되었습니다.
- 여기부터는 제 번역본을 읽어보시면 도움이 될 것 같습니다.
<br>

#### [05_sprite_raycast](https://github.com/yohan9612/cub3d/blob/master/mlx_example/05_sprite_raycast.c)

``` C
//loop through every vertical stripe of the sprite on screen
for(int stripe = drawStartX; stripe < drawEndX; stripe++)
{
	...
	...

	for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
	{
		int d = (y-vMoveScreen) * 256 - height * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
		int texY = ((d * texHeight) / spriteHeight) / 256;
		int color = info->texture[sprite[spriteOrder[i]].texture][texWidth * texY + texX]; //get current color from the texture
		if((color & 0x00FFFFFF) != 0) info->buf[y][stripe] = color; //paint pixel if it isn't black, black is the invisible color
	}
}
```

- sprite를 처리하는 부분에서도 대부분 이전 코드와 동일하지만, sprite들을 정렬하고, 이미지에 담는 코드가 추가되었습니다.

---
### 번역안내

- [__Lode's Computer Graphics Tutorial__ (튜토리얼 원문)](https://lodev.org/cgtutor/raycasting.html)
- [(저작권 안내)](https://lodev.org/cgtutor/legal.html) 
