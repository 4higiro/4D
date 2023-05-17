<h1>Визуализатор тессеракта</h1>
<p>Простенький графический движок, полностью реализованный во фрагментном шейдере (в <a href="sources/main.cpp">main.cpp</a> только управление камерой) с пмомщью алгоритма рендеринга - Raycasting</p>
<p>Рендеряться 8 четырехмерных гиперплоскостей, ограниченных точками (0, 0, 0, 0)<sup>T</sup> и (1, 1, 1, 1)<sup>T</sup> по диагонали. Эти поверхности отображаются сечением трехмерным подпространством с помощью дискретизации четвертой координаты. Возможны так же линейные преобразования над вершинами тессеракта в четвертом измерении.</p>
<h2>Демонстарционная версия выглядит так:</h2>
<img src="demo.gif" width=500px>
<p>Разработка велась на языке С++ при использовании библиотеки двухмерной графики SFML.</p>
