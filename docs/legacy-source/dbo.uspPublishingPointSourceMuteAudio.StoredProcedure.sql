/****** Object:  StoredProcedure [dbo].[uspPublishingPointSourceMuteAudio]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE procedure [dbo].[uspPublishingPointSourceMuteAudio] 
( 
	@PptParticipantID	uniqueidentifier 
	, @MuteFlag			bit  
) 
AS
BEGIN

	
	DECLARE	@_error int
	DECLARE @_errorString char( 255 )

	DECLARE @PublishingPointID uniqueidentifier
	DECLARE @RowID uniqueidentifier

	SELECT 
		@RowId = PPS.ID 
		, @PublishingPointID = C.ID
	FROM utConferenceParticipant AS CP 
		INNER JOIN utConference  AS C 
			ON CP.ConferenceID = C.ID 
		INNER JOIN utPublishingPointSource PPS 
			ON CP.ID = PPS.ConferenceParticipantID
	WHERE PPS.ConferenceParticipantID = @PptParticipantID
			AND PPS.avSourceType = 1

	IF 0 = @@ROWCOUNT
	BEGIN
		SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @PptParticipantID ) )
		RAISERROR( 
			'PptParticipant does not exist for [uspPublishingPointSourceRemoveAudio]: %d '
			, 16
			, 1
			, 0
			, @_errorString
			) WITH LOG
		RETURN -1
	END

BEGIN DISTRIBUTED TRANSACTION

	UPDATE utPublishingPointSource
	SET isActive = 1
		, isStreamable = @MuteFlag
	WHERE ID = @RowId

	IF 0 = @@ROWCOUNT
	BEGIN
		SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @rowId ) )
		RAISERROR( 
			'PptParticipant isActive failed in [uspPublishingPointSourceRemoveAudio]: %d '
			, 16
			, 1
			, 0
			, @_errorString
			) WITH LOG
		ROLLBACK TRANSACTION
		RETURN -1
	END


--	SET @_cdrID = newid()
--	complete CDR stuff once datamodel is finalized.

	DECLARE @mgmtServer nvarchar( 50 )
	DECLARE @mgmtServerQ nvarchar( 100 )
	DECLARE @QueueName nvarchar( 255 )
	DECLARE @TriggerId uniqueidentifier
	DECLARE @EventType int
	DECLARE @EventTypeId int

	SET @EventType = 10001 -- BET_SERVER_ALL_UDP
	IF @MuteFlag = 0
	BEGIN
		SET @EventTypeId = 10009 -- BE_REMOVE_PPT_AUDIO_SOURCE_RT
	END
	ELSE
	BEGIN
		SET @EventTypeId = 10003 -- BE_INSERT_PPT_AUDIO_SOURCE_RT
	END

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
						, @MessageLabel			= N'Mute Audio Source RT' 
						, @TriggerId			= @TriggerId
						, @EventType			= @EventType
						, @EventTypeId			= @EventTypeId
						, @PublishingPointId	= @PublishingPointId
						, @PptParticipantId		= @PptParticipantId
						, @RowId				= @RowId

		IF 1 < @_error
		BEGIN

			SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @PptParticipantId ) )
			RAISERROR( 
				'Mute Audio for PublishingPointSource failed with error code of: %d for PublishingPoint Participant: %s'
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
