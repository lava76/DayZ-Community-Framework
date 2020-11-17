modded class DayZGame
{
    override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
    {
        //Intercept incoming RPCs and see if they carry RPC type id.
        if(CF_RPC._OnRPC(sender, target, rpc_type, ctx)) return;

        super.OnRPC(sender, target, rpc_type, ctx);
    }
}
