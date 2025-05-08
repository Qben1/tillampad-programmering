require 'ruby2d'

set width: 1024, height: 512
set title: "Golf Game"

$goal_x = rand(1024)
$goal_y = rand(512)

def draw_goal
    Circle.new(
        x: $goal_x,
        y: $goal_y,
        radius: 20,
        sectors: 32,
        color: 'red'
    )
end

def ball_hits_goal?(ball_x, ball_y)
    radius = 20
    ball_x.between?($goal_x - radius, $goal_x + radius) && ball_y.between?($goal_y - radius, $goal_y + radius)
end

class Ball
    attr_reader :x, :y

    def initialize
        @x = 512
        @y = 256
        @vx = 0   # Velocity X
        @vy = 0   # Velocity Y
        @friction = 0.98  # Slowdown factor
        @speed_multiplier = 0.05  # Adjust for realistic speed
    end

    def draw
        Circle.new(
            x: @x, y: @y, 
            radius: 10,
            color: 'white',
            z: 10
        )
    end

    def bounce
        if @x <= 0
            @vx = -@vx
            @x = 1
        elsif @x >= 1024
            @vx = -@vx
            @x = 1023
        end

        if @y <= 0
            @vy = -@vy
            @y = 1
        elsif @y >= 512
            @vy = -@vy
            @y = 511
        end
    end

    def putt(target_x, target_y)
        @vx = (target_x - @x) * @speed_multiplier
        @vy = (target_y - @y) * @speed_multiplier
    end

    def move
        @x -= @vx
        @y -= @vy

        # Apply friction to slow the ball down
        @vx *= @friction
        @vy *= @friction

        # Stop the ball when velocity is very low
        if @vx.abs < 0.1 && @vy.abs < 0.1
            @vx = 0
            @vy = 0
        end
    end

    def stopped?
        @vx == 0 && @vy == 0
    end
end

ball = Ball.new

score = 0
goal_score = 10
shots_left = 3
game_over = false
end_time = nil
show_restart_button = false

on :mouse_up do
    if game_over == false && score < goal_score
        if ball.stopped?
            ball.putt(Window.mouse_x, Window.mouse_y)
            shots_left -= 1
        end
    else
        if show_restart_button
            if Window.mouse_x.between?(450, 570) && Window.mouse_y.between?(250, 290)
                score = 0
                shots_left = 3
                $goal_x = rand(1024)
                $goal_y = rand(512)
                game_over = false
                show_restart_button = false
                ball = Ball.new
            end
        end
    end
end

update do
    clear
    ball.move

    Text.new("Shots left: #{shots_left}", x: 10, y: 10, size: 20, color: 'white', z: 100)
    Text.new("Score: #{score}", x: 900, y: 10, size: 20, color: 'white', z: 100)

    draw_goal
    ball.draw

    if game_over
        if score >= goal_score
            message = "YOU WIN!"
        else
            message = "YOU LOSE! Score: #{score}"
        end

        Text.new(message, x: (1024 - message.length * 20) / 2, y: 180, size: 40, color: 'yellow', z: 200)
      
        if show_restart_button
            Rectangle.new(x: 450, y: 250, width: 120, height: 40, color: 'gray', z: 210)
            Text.new("Play Again", x: 460, y: 260, size: 20, color: 'white', z: 220)
        end
    end
    
    ball.stopped?
    ball.bounce

    if ball.stopped? && ball_hits_goal?(ball.x, ball.y)
        score += 1
        $goal_x = rand(1024)
        $goal_y = rand(512)
        shots_left = 3
    end

    if ball.stopped? && !game_over
        if score >= goal_score || shots_left <= 0
          game_over = true
          show_restart_button = true
        end
    end
end

show