while 1
    pause(0.05)
    theta = getTxtData(string('pnp_theta.txt'));
    theta = theta / 180 * pi;
    T = getTxtData(string('pnp_t.txt'));
    clf;

    World = rotx(0);
    Camera = trotx(theta(1))*troty(theta(2))*trotz(theta(3));
    Camera = transl(T) * Camera;
    axis_size = [-500 500 -500 500 -500 500];
    trplot(Camera, 'length', 100, 'frame', 'C', 'text_opts', ...,
    {'FontSize', 10, 'FontWeight', 'bold'}, 'rgb', 'axis', axis_size);
    hold on;
    trplot(World, 'length', 100, 'rgb', 'axis', axis_size);

    plot3(0, 0, 0, 'r.','MarkerSize',20)
    plot3(0, 200, 0, 'r.','MarkerSize',20)
    plot3(150, 0, 0, 'r.','MarkerSize',20)
    plot3(150, 200, 0, 'r.','MarkerSize',20)
    plot3(T(1), T(2), T(3), 'b.', 'MarkerSize',20);
end



