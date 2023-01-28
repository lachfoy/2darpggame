texture = "images/enemy.png"
speed = 30.0
friction = 20.0
damage = 10
max_health = 50
detect_range = 5.0
collider_radius = 2.0

enemy_behaviour = coroutine.create(function (enemy)
    actions = {
        function () MoveTo(enemy,  0,  1, 1, 0.5) coroutine.yield() Wait(enemy, 0.5) end, -- move up
        function () MoveTo(enemy,  0, -1, 1, 0.5) coroutine.yield() Wait(enemy, 0.5) end, -- move down
        function () MoveTo(enemy, -1,  0, 1, 0.5) coroutine.yield() Wait(enemy, 0.5) end, -- move left
        function () MoveTo(enemy,  1,  0, 1, 0.5) coroutine.yield() Wait(enemy, 0.5) end, -- move right
    }

    while true do
        actions[math.random(#actions)]()
        coroutine.yield()
    end


end)

function start_next_behaviour(enemy)
    if coroutine.status(enemy_behaviour) ~= 'dead' then
        coroutine.resume(enemy_behaviour, enemy)
    end
end
