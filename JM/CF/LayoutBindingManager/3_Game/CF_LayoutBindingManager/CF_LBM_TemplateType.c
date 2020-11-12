// Gets typename from Templated type
// This is useful when getting this error...
// 'Can't get typename from forward declaration 'Class'. Use Type("name") instead....'
class CF_LBM_TemplateType<Class T>
{
	T value;

	static typename GetType()
	{
		return (new CF_LBM_TemplateType<T>()).Type().GetVariableType(0);
	}
};