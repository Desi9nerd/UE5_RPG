using UnrealBuildTool;

public class RPG : ModuleRules
{
	public RPG(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivateDependencyModuleNames.AddRange(new string[] { "GameplayCameras" });
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);


        PublicDependencyModuleNames.Add("Core");

        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("InputCore");
        PrivateDependencyModuleNames.Add("Niagara");
    }
}
