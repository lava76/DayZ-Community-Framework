class CF_RPC_Context extends ScriptRPC
{
	protected bool m_Guaranteed;
	protected Object m_Target;

    /**
     * @brief Sends the prepared RPC context to the recipient machine. You can re-send the RPC to the same or a different recipient by calling SendTo() again.
     * @code
     * auto rpc = CF.RPC.Prepare("MyHandler", "MyFunction", true);
     * rpc.SendTo(player.GetIdentity());
     * rpc.SendTo(player2.GetIdentity());
     * @endcode
     *
     *
     * @param instance  The handler instance to be registered
     * @return bool     Returns true if successfully registered, false otherwise (see script log for details).
     */
    void SendTo(PlayerIdentity recipient = null)
    {
        Send(m_Target, CF_RPC.CF_RPC_SCRIPT_ID, m_Guaranteed, recipient);
    }

    /**
     * @brief [Internal] Prepares a new RPC context. Used by CF.RPC.Prepare()
     *
     * @return CF_RPC_Context RPC write and send context.
     */
    static ref CF_RPC_Context _Prepare(string handlerType, string functionName, bool guaranteed, Object target)
    {
        CF_RPC_Context ctx();
        ctx.m_Guaranteed = guaranteed;
        ctx.m_Target = target;

        ctx.Write(handlerType);
        ctx.Write(functionName);

        return ctx;
    }
}
