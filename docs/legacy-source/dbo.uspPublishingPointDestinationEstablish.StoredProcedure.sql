/****** Object:  StoredProcedure [dbo].[uspPublishingPointDestinationEstablish]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Description:	Establish a video destination for a Publishing Point
-- =============================================
CREATE PROCEDURE [dbo].[uspPublishingPointDestinationEstablish]
(
	@ConferenceID uniqueidentifier 
	, @ConferenceParticipantID uniqueidentifier 
	, @ipVerLen smallint
	, @ipTos smallint
	, @ipTotalLength int
	, @ipId int
	, @ipOffset int
	, @ipTtl smallint
	, @ipProtocol smallint
	, @srcIP bigint
	, @destIP bigint
	, @srcPort int
	, @destPort int
	, @udpLength int
	, @MacAddress char( 24 )
)
AS
BEGIN

BEGIN DISTRIBUTED TRANSACTION

	SET NOCOUNT ON;

	DECLARE	@_error int
	DECLARE @_errorString char( 255 )

	DECLARE @_rowCount int

	DECLARE	@_MacAddress char( 24 )
	DECLARE @_pptdID uniqueidentifier
	DECLARE @_cdrID uniqueidentifier

	SET @_error = 0
	SET @_rowCount = 0

	SET @MacAddress = LTRIM( RTRIM( @MacAddress ) )

--NOTE:  Need to declare error codes and do this correctly...

	IF NOT EXISTS
	( 
		SELECT TOP 1 PPTP.ID
		FROM utConferenceParticipant AS PPTP
		WHERE PPTP.ID = @ConferenceParticipantID
	)
	BEGIN
		
		SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @ConferenceParticipantID ) )
		RAISERROR( 
			'PublishingPoint Participant does not exist with id: %s'
			, 16
			, 1
			, @_errorString
			) WITH LOG
		ROLLBACK TRANSACTION
		RETURN -1

	END
	ELSE
	BEGIN

		SELECT @_MacAddress = LTRIM( RTRIM( PPTP.MacAddress ) )
		FROM utConferenceParticipant AS PPTP
		WHERE PPTP.ID = @ConferenceParticipantID

		IF @_MacAddress <> @MacAddress
		BEGIN
			SET @_MacAddress = @MacAddress

			UPDATE utConferenceParticipant
			SET MacAddress = @MacAddress
			WHERE ID = @ConferenceParticipantID
		END

	END

	IF EXISTS
	(
		SELECT TOP 1 PPTD.ID
		FROM utPublishingPointDestination AS PPTD
		WHERE PPTD.ConferenceParticipantID = @ConferenceParticipantID
	)
	BEGIN
	
		SELECT @_pptdID = PPTD.ID
		FROM utPublishingPointDestination AS PPTD
		WHERE PPTD.ConferenceParticipantID = @ConferenceParticipantID

		EXECUTE @_error = uspUpdatePublishingPointDestination
				@_pptdID
				, @ConferenceParticipantID  
				, @ipVerLen 
				, @ipTos 
				, @ipTotalLength 
				, @ipId 
				, @ipOffset 
				, @ipTtl 
				, @ipProtocol 
				, @srcIP 
				, @destIP 
				, @srcPort 
				, @destPort 
				, @udpLength 
				, 1
				, 1

		IF 0 <> @_error
		BEGIN
			SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @ConferenceParticipantID ) )
			RAISERROR( 
				'Delete of PublishingPointDestination failed with error code of: %d for PublishingPoint Participant: %s'
				, 16
				, 1
				, @_error
				, @_errorString
				) WITH LOG
			ROLLBACK TRANSACTION
			RETURN -1
		END
		
	END
	ELSE
	BEGIN

		SET @_pptdID = newid()
		EXECUTE @_error = uspCreatePublishingPointDestination
				@_pptdID
				, @ConferenceParticipantID  
				, @ipVerLen 
				, @ipTos 
				, @ipTotalLength 
				, @ipId 
				, @ipOffset 
				, @ipTtl 
				, @ipProtocol 
				, @srcIP 
				, @destIP 
				, @srcPort 
				, @destPort 
				, @udpLength 
				, 1
				, 1

		IF 0 <> @_error
		BEGIN

			SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @ConferenceParticipantID ) )
			RAISERROR( 
				'Insert of PublishingPointDestination failed with error code of: %d for PublishingPoint Participant: %s'
				, 16
				, 1
				, @_error
				, @_errorString
				) WITH LOG
				ROLLBACK TRANSACTION
			RETURN -1

		END
	END

--	SET @_cdrID = newid()
--	complete CDR stuff once datamodel is finalized.


	DECLARE @mgmtServer nvarchar( 50 )
	DECLARE @mgmtServerQ nvarchar( 100 )
	DECLARE @QueueName nvarchar( 255 )

	SELECT
		@mgmtServer = S.Name
		, @mgmtServerQ = S.ManagementQueue
	FROM utServer as S
	WHERE S.ID = (
		SELECT TOP 1 S2.ID
		FROM utServer as S2
	)

	DECLARE @TriggerId uniqueidentifier
	DECLARE @EventType int
	DECLARE @EventTypeId int
	DECLARE @PublishingPointId uniqueidentifier
	DECLARE @PptParticipantId uniqueidentifier

	SET @TriggerId = newid()
	SET @EventType = 10000 -- BET_SERVER
	SET @EventTypeId = 10004 -- BE_INSERT_PPT_MEDIA_DESTINATION
	SET @PublishingPointId = @ConferenceID
	SET @PptParticipantId = @ConferenceParticipantID

	SET @QueueName = N'Direct=OS:' 
						+ LTRIM( RTRIM ( @mgmtServer ) ) 
						+ N'\private$\'
						+ LTRIM( RTRIM ( @mgmtServerQ ) )

	execute @_error = master.dbo.xp_blx_sqltriggerevent
					@QueueName = @QueueName
					, @MessageLabel = N'Add Media Destination' 
					, @TriggerId = @TriggerId
					, @EventType = @EventType
					, @EventTypeId = @EventTypeId
					, @PublishingPointId = @PublishingPointId
					, @PptParticipantId = @PptParticipantId
					, @RowId = @_pptdID

	IF 1 < @_error
	BEGIN

		SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @ConferenceParticipantID ) )
		RAISERROR( 
			'Publication Video destination for PublishingPointDestination failed with error code of: %d for PublishingPoint Participant: %s'
			, 16
			, 1
			, @_error
			, @_errorString
			) WITH LOG
		ROLLBACK TRANSACTION
		RETURN -1

	END

	--should go to all servers...
	SET @PublishingPointId = @ConferenceID
	SET @PptParticipantId = @ConferenceParticipantID


	DECLARE @ServerName nvarchar( 50 )
			, @ManagementQueue nvarchar( 50 )
			, @ServerType int

	DECLARE utServer_cursor CURSOR 
		LOCAL FORWARD_ONLY FAST_FORWARD READ_ONLY FOR
		SELECT S.Name
				, S.ManagementQueue
				, S.ServerType
		FROM utServer AS S
		WHERE S.ServerType = 0

	OPEN utServer_cursor

	SET @EventType = 10001 -- BET_SERVER_ALL_UDP
	SET @EventTypeId = 10005 -- BE_INSERT_PPT_MEDIA_DESTINATION_RT

	FETCH utServer_cursor
	INTO @ServerName
		, @ManagementQueue
		, @ServerType

	WHILE @@FETCH_STATUS = 0
	BEGIN

		SET @TriggerId = newid()
		
		SET @QueueName = N'Direct=OS:' 
						+ LTRIM( RTRIM ( @ServerName ) ) 
						+ N'\private$\'
						+ LTRIM( RTRIM ( @ManagementQueue ) )

		EXECUTE @_error = master.dbo.xp_blx_sqltriggerevent
						@QueueName				= @QueueName
						, @MessageLabel			= N'Sync establish media destination RT' 
						, @TriggerId			= @TriggerId
						, @EventType			= @EventType
						, @EventTypeId			= @EventTypeId
						, @PublishingPointId	= @PublishingPointId
						, @PptParticipantId		= @PptParticipantId
						, @RowId				= @_pptdID

		IF 1 < @_error
		BEGIN

			SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @PptParticipantId ) )
			RAISERROR( 
				'Sync media destination PublishingPointDestination failed with error code of: %d for PublishingPoint Participant: %s'
				, 16
				, 1
				, @_error
				, @_errorString
				) WITH LOG
			ROLLBACK TRANSACTION
			RETURN -1

		END

		FETCH utServer_cursor
		INTO @ServerName
			, @ManagementQueue
			, @ServerType

	END

COMMIT TRANSACTION

CLOSE utServer_cursor;
DEALLOCATE utServer_cursor;

	RETURN 0
END
GO
