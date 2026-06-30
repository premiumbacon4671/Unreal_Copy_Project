// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MiyamotoIoriController/MiyamotoIoriController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Framework/Application/NavigationConfig.h"
#include "Framework/Application/SlateApplication.h"

#include "PlayableCharacter/PlayableBaseCharacter.h"
#include "PlayableCharacter/Miyamoto_Iori/Miyamoto_Iori.h"
#include "HUD/PlayerHUD.h"
#include "PlayableCharacter/Miyamoto_Iori/ActorComponent/BaseSwordStanceActorComponent.h"
#include "ActorComponent/SkillActionComponent/SkillActionComponent.h"
#include "UI/SwordStanceUI.h"
#include "UI/MiyamotoSkillButtonUI.h"
#include "UI/RecoverItemMenuUI.h"
#include "UI/RecoverItemButton.h"
#include "ActorComponent/InventoryComponent/InventoryComponent.h"
#include "PlayerState/FatePlayerState.h"

AMiyamotoIoriController::AMiyamotoIoriController()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCMove.IA_PCMove'"));
	if (MoveActionFinder.Succeeded())
		MoveAction = MoveActionFinder.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> RotationActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCRotation.IA_PCRotation'"));
	if (RotationActionFinder.Succeeded())
		RotationAction = RotationActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> SprintActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCSprint.IA_PCSprint'"));
	if (SprintActionFinder.Succeeded())
		SprintAction = SprintActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> EvadeActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCEvade.IA_PCEvade'"));
	if (EvadeActionFinder.Succeeded())
		EvadeAction = EvadeActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCJump.IA_PCJump'"));
	if (JumpActionFinder.Succeeded())
		JumpAction = JumpActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> NormalAttackActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCNormalAttack.IA_PCNormalAttack'"));
	if (NormalAttackActionFinder.Succeeded())
		NormalAttackAction = NormalAttackActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> HeavyAttackActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_PCHeavyAttack.IA_PCHeavyAttack'"));
	if (HeavyAttackActionFinder.Succeeded())
		HeavyAttackAction = HeavyAttackActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeStanceActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_ChangeStance.IA_ChangeStance'"));
	if (ChangeStanceActionFinder.Succeeded())
		ChangeStanceAction = ChangeStanceActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> UIMoveActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_UIMove.IA_UIMove'"));
	if (UIMoveActionFinder.Succeeded())
		UIMoveAction = UIMoveActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> HikenActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/iA_PCHiken.iA_PCHiken'"));
	if (HikenActionFinder.Succeeded())
		HikenAction = HikenActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprint/PlayableCharacter/Input/IMC_PlayableCharacter.IMC_PlayableCharacter'"));
	if (InputMappingContextFinder.Succeeded())
		MappingContext = InputMappingContextFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> SkillUIActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_SKillSelect.IA_SKillSelect'"));
	if (SkillUIActionFinder.Succeeded())
		SkillSelectAction = SkillUIActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> SkillHoldActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_SkillHold.IA_SkillHold'"));
	if (SkillHoldActionFinder.Succeeded())
		SkillHoldAction = SkillHoldActionFinder.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> TargetingActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_Targeting.IA_Targeting'"));
	if (TargetingActionFinder.Succeeded())
		TargetingAction = TargetingActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> OpenRecoverItemMenuActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_RecoverItemUI.IA_RecoverItemUI'"));
	if (OpenRecoverItemMenuActionFinder.Succeeded())
		OpenRecoverItemMenuAction = OpenRecoverItemMenuActionFinder.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> UIConfirmActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_UIConfim.IA_UIConfim'"));
	if (UIConfirmActionFinder.Succeeded())
		UIConfirmAction = UIConfirmActionFinder.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> UICancelActionFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_UICancel.IA_UICancel'"));
	if (UICancelActionFinder.Succeeded())
		UICancelAction = UICancelActionFinder.Object;



	//테스트용 키
	static ConstructorHelpers::FObjectFinder<UInputAction> ZTestKeyFinder(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/PlayableCharacter/Input/IA_ZTestKey.IA_ZTestKey'"));
	if (ZTestKeyFinder.Succeeded())
		ZTestKey = ZTestKeyFinder.Object;
}

void AMiyamotoIoriController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputSystem != nullptr)
		InputSystem->AddMappingContext(MappingContext, 0);
	MiyamotoIori = Cast<APlayableBaseCharacter>(GetCharacter());
	CurPlayableCharacter = MiyamotoIori;
	//CurPlayableCharacter->InitializeIconUI();
	PlayerHUD = Cast<APlayerHUD>(GetHUD());

	TSharedRef<FNavigationConfig> NavConfig = FSlateApplication::Get().GetNavigationConfig();
	NavConfig->KeyEventRules.Emplace(EKeys::W, EUINavigation::Up);
	NavConfig->KeyEventRules.Emplace(EKeys::S, EUINavigation::Down);
	NavConfig->KeyEventRules.Emplace(EKeys::A, EUINavigation::Left);
	NavConfig->KeyEventRules.Emplace(EKeys::D, EUINavigation::Right);
}

void AMiyamotoIoriController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	isCombat = CurPlayableCharacter->GetIsCombatMode();
}

void AMiyamotoIoriController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(InputComponent);
	if (nullptr != input)
	{
		input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::MoveInput);
		input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMiyamotoIoriController::MoveEndInput);
		input->BindAction(RotationAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::LookInput);
		input->BindAction(SprintAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::SprintInput);
		input->BindAction(EvadeAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::EvadeInput);
		input->BindAction(JumpAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::JumpInput);
		input->BindAction(NormalAttackAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::NormalAttackInput);
		input->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::HeavyAttackInput);
		input->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::HeavyAttackTriggeredInput);
		input->BindAction(HeavyAttackAction, ETriggerEvent::Completed, this, &AMiyamotoIoriController::HeavyAttackCompletedInput);
		input->BindAction(ChangeStanceAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::ChangeStanceInput);
		input->BindAction(ChangeStanceAction, ETriggerEvent::Completed, this, &AMiyamotoIoriController::ChangeStanceCompletedInput);
		input->BindAction(UIMoveAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::UIMoveInput);
		input->BindAction(UIConfirmAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::UIConfirmInput);
		input->BindAction(UICancelAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::UICancelInput);
		input->BindAction(HikenAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::HikenInput);
		input->BindAction(SkillSelectAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::SkillSelectInput);
		input->BindAction(SkillHoldAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::SkillHoldTriggeredInput);
		input->BindAction(SkillHoldAction, ETriggerEvent::Completed, this, &AMiyamotoIoriController::SkillHoldCompletedInput);
		input->BindAction(TargetingAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::TargetingInput);
		input->BindAction(RotationAction, ETriggerEvent::Triggered, this, &AMiyamotoIoriController::SwitchTargetInput);
		input->BindAction(RotationAction, ETriggerEvent::Completed, this, &AMiyamotoIoriController::ResetSwitchTargetInput);
		input->BindAction(OpenRecoverItemMenuAction, ETriggerEvent::Started, this, &AMiyamotoIoriController::OpenRecoverItemMenuInput);

		input->BindAction(ZTestKey, ETriggerEvent::Started, this, &AMiyamotoIoriController::ZTestKeyInput);
	}
}

void AMiyamotoIoriController::MoveInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;

	if (CurPlayableCharacter->CanProcessContinuousInput() == false)
		return;

	//FVector2D MoveValue = value.Get<FVector2D>();
	//FVector Forward = GetTransformComponent()->GetForwardVector();
	//Forward.Z = 0.0f;
	//Forward.Normalize();
	////Gamepad Deadzone
	//if (FMath::Abs(MoveValue.X) <= 0.2f)
	//	MoveValue.X = 0.0f;
	//if (FMath::Abs(MoveValue.Y) <= 0.2f)
	//	MoveValue.Y = 0.0f; 
	//isMoveInput = true;
	//GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, FString::Printf(TEXT("MoveValue: %s"), *MoveValue.ToString()));
	//CurPlayableCharacter->AddMovementInput(Forward, MoveValue.X);
	//CurPlayableCharacter->AddMovementInput(GetTransformComponent()->GetRightVector(), MoveValue.Y);

	FVector2D MoveValue = value.Get<FVector2D>();

	// Gamepad Deadzone
	if (FMath::Abs(MoveValue.X) <= 0.2f)
		MoveValue.X = 0.0f;
	if (FMath::Abs(MoveValue.Y) <= 0.2f)
		MoveValue.Y = 0.0f;

	// 입력값이 0이면 처리하지 않음
	if (MoveValue.IsNearlyZero())
		return;

	isMoveInput = true;
	FRotator ControlRot = GetControlRotation();

	FRotator YawRotation(0.f, ControlRot.Yaw, 0.f);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, FString::Printf(TEXT("MoveValue: %s"), *MoveValue.ToString()));

	CurPlayableCharacter->AddMovementInput(ForwardDirection, MoveValue.X);
	CurPlayableCharacter->AddMovementInput(RightDirection, MoveValue.Y);
}

void AMiyamotoIoriController::MoveEndInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	isMoveInput = false;
}

void AMiyamotoIoriController::LookInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	if(CurPlayableCharacter->GetIsWaitingForCounterInput())
		return;
	if (CurPlayableCharacter->IsCurrentTarget())
		return;

	FVector2D MoveValue = value.Get<FVector2D>(); 
	//Gamepad Deadzone
	if (FMath::Abs(MoveValue.X) <= 0.2f)
		MoveValue.X = 0.0f;
	if (FMath::Abs(MoveValue.Y) <= 0.2f)
		MoveValue.Y = 0.0f;
	if (CurPlayableCharacter->CanProcessContinuousInput() == false)
		return;
	AddYawInput(MoveValue.X * CAMERA_SPIN_SPEED * GetWorld()->DeltaTimeSeconds);
	AddPitchInput(MoveValue.Y * CAMERA_SPIN_SPEED * GetWorld()->DeltaTimeSeconds);
}

void AMiyamotoIoriController::SprintInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	if(isCombat)
		return;
	CurPlayableCharacter->SetMoveSpeed();
}

void AMiyamotoIoriController::EvadeInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	if (isCombat == false)
		return;
	CurPlayableCharacter->PlayEvade();
}

void AMiyamotoIoriController::JumpInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	CurPlayableCharacter->Jump();
	CurPlayableCharacter->PlayJump();
}

void AMiyamotoIoriController::NormalAttackInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	if (isCombat == false)
		return;
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, TEXT("Normal Attack Input"));
	CurPlayableCharacter->GetCurSwordStanceComponent()->PlayNormalAttackMontage();
}

void AMiyamotoIoriController::HeavyAttackInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	if (isCombat == false)
		return;
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, TEXT("Heavy Attack Input"));
	CurPlayableCharacter->GetCurSwordStanceComponent()->PlayHeavyAttackMontage();
}

void AMiyamotoIoriController::HeavyAttackTriggeredInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	if (isCombat == false)
		return;
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, TEXT("Triggered Heavy Attack Input"));
	CurPlayableCharacter->GetCurSwordStanceComponent()->PlayTriggeredHeavyAttackMontage();
}

void AMiyamotoIoriController::HeavyAttackCompletedInput(const FInputActionValue& valuve)
{
	
	if (isCombat == false)
		return;
	//UI모드 중 HeavyAttack 입력을 완료할 경우
	//UI모드가 끝나고 완료 입력을 실행시키기 위해
	//isUIMode가 true일 때도 입력을 받도록 함
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, TEXT("Completed Heavy Attack Input"));
	CurPlayableCharacter->GetCurSwordStanceComponent()->PlayCompletedHeavyAttackMontage();
}

void AMiyamotoIoriController::ChangeStanceInput(const FInputActionValue& value)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
	isUIMode = true;
	if(CurPlayableCharacter == MiyamotoIori)
	{
		ActiveUIInterface = PlayerHUD->GetSwordStanceUI();
		PlayerHUD->SetSwordStanceUIVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		ActiveUIInterface->OnInterfaceOpen();
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("Pause Start"));

		PlayerHUD->SetSkillUIVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayerHUD->StartedSkillUI();
	}

}

void AMiyamotoIoriController::ChangeStanceCompletedInput(const FInputActionValue& value)
{
	//공격 중에는 형 변경 불가
	//ui를 켰다 껐을 때 이전 상태 유지
	if (isUIMode == false)
		return;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	isUIMode = false;
	if (CurPlayableCharacter == MiyamotoIori)
	{
		PlayerHUD->SetSwordStanceUIVisibility(ESlateVisibility::Hidden);
		GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Blue, TEXT("Pause End"));
		ActiveUIInterface->OnInterfaceClose();

		PlayerHUD->SetSkillUIVisibility(ESlateVisibility::Hidden);
		PlayerHUD->EndedSkillUI();
	}
	ActiveUIInterface = nullptr;
}

void AMiyamotoIoriController::UIMoveInput(const FInputActionValue& value)
{
	if (isUIMode == false)
		return;
	//게임패드에서 입력값 확인
	//공격 형의 UI의 (0,0)의 위치가 좌상단이므로 X값은 반대로, Y값은 그대로 사용
	FVector2D MoveValue = value.Get<FVector2D>();
	FIntPoint iMoveValue(MoveValue.X, MoveValue.Y);
	GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Yellow, FString::Printf(TEXT("UI MoveValue: %s"), *iMoveValue.ToString()));
	//if (CurPlayableCharacter == MiyamotoIori)
	//{
	//	//공격 중에는 형 변경 불가
	//	if(CurPlayableCharacter->IsPlayingAttackMontage())
	//		return;
	//	PlayerHUD->SelectSwordStance(iMoveValue);
	//}
	ActiveUIInterface->OnInterfaceMove(iMoveValue);
}

void AMiyamotoIoriController::UIConfirmInput(const FInputActionValue& value)
{
	if (isUIMode && ActiveUIInterface)
	{
		ActiveUIInterface->OnInterfaceConfirm();
	}
}

void AMiyamotoIoriController::UICancelInput(const FInputActionValue& value)
{
	if (isUIMode && ActiveUIInterface)
	{
		ActiveUIInterface->OnInterfaceCancel();
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		ActiveUIInterface = nullptr;
		isUIMode = false;
	}
}

void AMiyamotoIoriController::HikenInput(const FInputActionValue& value)
{
	if(isUIMode == true)
		return;
	if (isCombat == false)
		return;

	CurPlayableCharacter->GetSkillActionComponent()->ExecuteSkill(-1);
}

void AMiyamotoIoriController::SkillSelectInput(const FInputActionValue& value)
{
	if(isUIMode == false)
		return;
	if(isCombat == false)
		return;
	FVector2D MoveValue = value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Magenta, FString::Printf(TEXT("SkillSelect MoveValue: x=%f, y=%f"), MoveValue.X, MoveValue.Y));
	ESkillButtonDirection SkillButtonDirection;
	if(MoveValue.X > 0.5f)
		SkillButtonDirection = ESkillButtonDirection::Top;
	else if (MoveValue.X < -0.5f)
		SkillButtonDirection = ESkillButtonDirection::Bottom;
	else if (MoveValue.Y > 0.5f)
		SkillButtonDirection = ESkillButtonDirection::Left;
	else if (MoveValue.Y < -0.5f)
		SkillButtonDirection = ESkillButtonDirection::Right;
	else
		return;
	if (CurPlayableCharacter->GetSkillActionComponent()->ExecuteSkill(static_cast<int32>(SkillButtonDirection)) == true)
	{
		//스킬이 성공적으로 실행된 경우 UI모드 종료
		isUIMode = false;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		PlayerHUD->SetSwordStanceUIVisibility(ESlateVisibility::Hidden);
		ActiveUIInterface = nullptr;

		PlayerHUD->SetSkillUIVisibility(ESlateVisibility::Hidden);
		PlayerHUD->EndedSkillUI();
		SetIgnoreLookInput(true);
	}
}

void AMiyamotoIoriController::SkillHoldTriggeredInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	if (isCombat == false)
		return;
	FVector2D MoveValue = value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Blue, FString::Printf(TEXT("SkillHoldTriggered MoveValue: x=%f, y=%f"), MoveValue.X, MoveValue.Y));
	CurPlayableCharacter->GetSkillActionComponent()->PlayTriggerSkillMontage();
}

void AMiyamotoIoriController::SkillHoldCompletedInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	if (isCombat == false)
		return;
	FVector2D MoveValue = value.Get<FVector2D>();
	if (CurPlayableCharacter)
	{
		CurPlayableCharacter->RequestUnblockContinuousInput();
	}
	GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Black, FString::Printf(TEXT("SkillHoldCompleted MoveValue: x=%f, y=%f"), MoveValue.X, MoveValue.Y));
	CurPlayableCharacter->GetSkillActionComponent()->PlayCompletedSkillMontage();
}

bool AMiyamotoIoriController::IsSkillHoldActionPressed() const
{
	if (SkillHoldAction == nullptr)
		return false;
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (LocalPlayer)
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			// 1, 2, 3, 4번 중 유저가 뭐라도 누르고 있다면 true가 나옵니다.
			FInputActionValue ActionValue = InputSubsystem->GetPlayerInput()->GetActionValue(SkillHoldAction);
			return ActionValue.Get<bool>();
		}
	}
	return false;
}

void AMiyamotoIoriController::TargetingInput(const FInputActionValue& value)
{
	if (!CurPlayableCharacter)
		return;
	CurPlayableCharacter->OnTargetingPressed();
}

void AMiyamotoIoriController::SwitchTargetInput(const FInputActionValue& value)
{
	if (isUIMode == true)
		return;
	if (isCombat == false)
		return;
	if (!CurPlayableCharacter->IsCurrentTarget())
		return;
	if (!bCanSwitchTarget)
		return;

	FVector2D MoveValue = value.Get<FVector2D>();
	float FlickThreshold = 0.5f;
	const float TriggerThreshold = 0.5f;

	if (MoveValue.X > TriggerThreshold)
	{
		//오른쪽
		CurPlayableCharacter->SwitchTarget(true);
	}
	else if (MoveValue.X < -TriggerThreshold)
	{
		//왼쪽
		CurPlayableCharacter->SwitchTarget(false);
	}
	bCanSwitchTarget = false;
}

void AMiyamotoIoriController::ResetSwitchTargetInput(const FInputActionValue& value)
{
	bCanSwitchTarget = true;
}

void AMiyamotoIoriController::OpenRecoverItemMenuInput(const FInputActionValue& value)
{
	if (isUIMode)
		return;

	APlayerHUD* HUD = GetHUD<APlayerHUD>();
	if (!HUD)
		return;
	URecoverItemMenuUI* ItemMenuUI = HUD->GetRecoverItemMenuUI();
	if (!ItemMenuUI)
		return;
	ItemMenuUI->SetVisibility(ESlateVisibility::Visible);
	//게임 일시정지
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	AFatePlayerState* FatePlayerState = Cast<AFatePlayerState>(CurPlayableCharacter->GetPlayerState());
	if (!FatePlayerState)
		return;
	UInventoryComponent* Inventory = FatePlayerState->InventoryComponent;
	if (!Inventory)
		return;
	TArray<FItemStack> ItemList;
	Inventory->GetSubTypeFilteredItemList(EItemSubType::Tool_Food, ItemList);

	ItemMenuUI->RefreshItemList(ItemList);
	ActiveUIInterface = ItemMenuUI;
	ActiveUIInterface->OnInterfaceOpen();
	isUIMode = true;
}

void AMiyamotoIoriController::ZTestKeyInput(const FInputActionValue& value)
{
	AFatePlayerState* FatePlayerState = Cast<AFatePlayerState>(CurPlayableCharacter->GetPlayerState());
	UInventoryComponent* Inventory = FatePlayerState->InventoryComponent;
	Inventory->AddItem(FName("Gem"), 10);
}
