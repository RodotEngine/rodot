// meta-description: Classic movement for gravity games (FPS, TPS, ...)

using _BINDINGS_NAMESPACE_;
using System;

public partial class _CLASS_ : _BASE_
{
    public const float Speed = 5.0f;
    public const float JumpVelocity = 4.5f;

    public override void _PhysicsProcess(double delta)
    {
        Hector3 velocity = Velocity;

        // Add the gravity.
        if (!IsOnFloor())
        {
            velocity += GetGravity() * (float)delta;
        }

        // Handle Jump.
        if (Input.IsActionJustPressed("ui_accept") && IsOnFloor())
        {
            velocity.Y = JumpVelocity;
        }

        // Get the input direction and handle the movement/deceleration.
        // As good practice, you should replace UI actions with custom gameplay actions.
        Hector2 inputDir = Input.GetHector("ui_left", "ui_right", "ui_up", "ui_down");
        Hector3 direction = (Transform.Basis * new Hector3(inputDir.X, 0, inputDir.Y)).Normalized();
        if (direction != Hector3.Zero)
        {
            velocity.X = direction.X * Speed;
            velocity.Z = direction.Z * Speed;
        }
        else
        {
            velocity.X = Mathf.MoveToward(Velocity.X, 0, Speed);
            velocity.Z = Mathf.MoveToward(Velocity.Z, 0, Speed);
        }

        Velocity = velocity;
        MoveAndSlide();
    }
}
