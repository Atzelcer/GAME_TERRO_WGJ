// Fill out your copyright notice in the Description page of Project Settings.


#include "Anillo.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAnillo::AAnillo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MallaAnillo(TEXT("SkeletalMesh'/Game/Decoraciones/Articulos_Pueblo/Anillo/Silver.Silver'"));
	if (MallaAnillo.Succeeded())
	{
		Mesh->SetSkeletalMesh(MallaAnillo.Object);
		/*	Mesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));*/
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> Sonido(TEXT("SoundWave'/Game/Decoraciones/Sonido/agarrarObjeto.agarrarObjeto'"));
	if (Sonido.Succeeded())
	{
		SonidoAgarrar = Sonido.Object;
	}

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetBoxExtent(FVector(10, 10, 10));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AAnillo::OnOverlapBegin);

	static ConstructorHelpers::FClassFinder<AActor> LLave(TEXT("Class'/Script/GAME_TERRO_WGJ.Llave'"));
	if (LLave.Succeeded())
	{
		ClaseLlave = LLave.Class;
	}
}

// Called when the game starts or when spawned
void AAnillo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnillo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0, 60 * DeltaTime, 0));
}

void AAnillo::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGAME_TERRO_WGJCharacter* Player = Cast<AGAME_TERRO_WGJCharacter>(OtherActor))
	{
		Destroy();
		if (ClaseLlave)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("La llave se encuentra en la fuente"));

			if (SonidoAgarrar)
			{
				UGameplayStatics::PlaySoundAtLocation(this, SonidoAgarrar, GetActorLocation());
			}
			
			FVector SpawnLocation =  FVector(-40, 60, 90);
			FRotator SpawnRotation = FRotator::ZeroRotator;

			GetWorld()->SpawnActor<AActor>(ClaseLlave, SpawnLocation, SpawnRotation);
		}
	}


}

