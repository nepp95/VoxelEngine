using EpEngine;
using System;

namespace Sandbox
{
    public class Camera : Entity
    {
        private Entity Player;
        public Entity OtherEntity;
        public float DistanceFromPlayer = 5.0f;

        void OnCreate()
        {
            Player = FindEntityByName("Player");
        }

        void OnUpdate(float ts)
        {
            if (Player != null)
            {
                Translation = new Vector3(Player.Translation.XY, DistanceFromPlayer);
            }

            Translation = new Vector3(Player.Translation.XY, DistanceFromPlayer);

            float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.S))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.D))
                velocity.X = 1.0f;

            velocity *= speed;

            Vector3 translation = Translation;
            translation += velocity * ts;
            Translation = translation;
        }
    }
}
