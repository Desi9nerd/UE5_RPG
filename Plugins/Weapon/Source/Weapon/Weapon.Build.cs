using UnrealBuildTool;

public class Weapon : ModuleRules
{
	public Weapon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.Add("Core");
        PrivateDependencyModuleNames.Add("RPG");//프로젝트명

        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("Slate");
        PrivateDependencyModuleNames.Add("SlateCore");

        PrivateDependencyModuleNames.Add("UnrealEd");//UFactory 
        PrivateDependencyModuleNames.Add("EditorStyle");
        PrivateDependencyModuleNames.Add("InputCore");

        PrivateDependencyModuleNames.Add("Projects");
        PrivateDependencyModuleNames.Add("EditorFramework");
        PrivateDependencyModuleNames.Add("ToolMenus");

    }
}
