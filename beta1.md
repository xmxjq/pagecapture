# Introduction #

实现了全部预期的功能

# Details #

> 走了一些弯路，开始妄图使用IDocHostUIHandler接口，来取消掉滚动条和3d边框，结果发现，必须刷新一下，才能应用，就放弃了，改为通过取值的方法忽略掉边框和滚动条 ，不过，也学会了如何在BHO中，应用IDocHostUIHandler接口，这个接口相关内容已经从代码中取消掉，需要的话，在svn中历史记录还有保留。

> 再就是在BitBlt中浪费很多时间，终于搞清楚了，BitBlt(img.GetDC(),pt.x, pt.y,tmpimg.cx,tmpimg.cy,hdc,srcpt.x,srcpt.y,SRCCOPY); 中CImage::GetDC()之后，还需要，使用CImage::ReleaseDC() 释放掉得到的DC，否则会弹出一个 DEBUG 警告框。