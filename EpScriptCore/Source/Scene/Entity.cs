namespace EpEngine
{
    public class Entity
    {
        protected Entity()
        {
            UUID = 0;
        }

        internal Entity(ulong uuid)
        {
            UUID = uuid;
        }

        public readonly ulong UUID;

        public Vector3 Translation
        {
            get
            {
                InternalCalls.Entity_GetTranslation(UUID, out Vector3 result);
                return result;
            }
            set
            {
                InternalCalls.Entity_SetTranslation(UUID, ref value);
            }
        }
    }
}