using EpEngine;
using System;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent m_transform;
        private RigidBodyComponent m_rigidBody;

        public float Speed;

        public void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {UUID}");

            m_transform = GetComponent<TransformComponent>();
            m_rigidBody = GetComponent<RigidBodyComponent>();

            Speed = 0.03f;
        }

        public void OnUpdate(float ts)
        {
            float speed = Speed;
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

            m_rigidBody.ApplyLinearImpulse(velocity.XY, true);

            Vector3 translation = m_transform.Translation;
            translation += velocity * ts;
            m_transform.Translation = translation;
        }
    }
}
