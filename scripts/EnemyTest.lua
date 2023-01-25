enemy_behaviour = coroutine.create(function (enemy)
    actions = {
        function () Move(enemy,  0,  1, 1, 0.5) coroutine.yield() Wait(enemy, 0.5) end, -- move up
        function () Move(enemy,  0, -1, 1, 0.5) coroutine.yield() Wait(enemy, 0.5) end, -- move down
        function () Move(enemy, -1,  0, 1, 0.5) coroutine.yield() Wait(enemy, 0.5) end, -- move left
        function () Move(enemy,  1,  0, 1, 0.5) coroutine.yield() Wait(enemy, 0.5) end, -- move right
        -- function () Shoot(enemy,  0,  1, 0.0) coroutine.yield() Wait(enemy, 1.0) end, -- shoot up
        -- function () Shoot(enemy,  0, -1, 0.0) coroutine.yield() Wait(enemy, 1.0) end, -- shoot down
        -- function () Shoot(enemy, -1,  0, 0.0) coroutine.yield() Wait(enemy, 1.0) end, -- shoot left
        -- function () Shoot(enemy,  1,  0, 0.0) coroutine.yield() Wait(enemy, 1.0) end  -- shoot right
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
