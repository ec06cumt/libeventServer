USE [uu278]
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
--�����б�
CREATE TABLE [tbluGuild]
(
	[GNum] [int] IDENTITY(1,1) NOT NULL,											--������
	[GName] [varchar](32) COLLATE Chinese_PRC_CI_AS NOT NULL,						--��������
	[GMark] [tinyint] NULL,															--����ͼ���ţ�������
	[GType] [tinyint] NULL,															--�������ͣ�������
	[GLevel] [tinyint] NOT NULL CONSTRAINT [DF_tbluGuild_GLevel] DEFAULT(1),		--����ȼ���Ĭ��1��
	[CreateTime] [datetime] NULL CONSTRAINT [DF_tbluGuild_CreateTime] DEFAULT(getdate()),--���崴��ʱ��Ĭ�ϵ�ǰʱ��
	[UpdateTime] [datetime] NULL,													--������Ϣ����ʱ��
	[bRefuseJoin] [tinyint] NOT NULL CONSTRAINT [DF_tbluGuild_GRefuse] DEFAULT(0),	--�Ƿ�ܾ���Ҽ������
	CONSTRAINT [PK_tbluGuild] PRIMARY KEY CLUSTERED									--��������
	(
		[GNum]
	)WITH FILLFACTOR = 90 ON [PRIMARY]												--�����������,�ӿ��ȡ�ٶ�
) ON [PRIMARY]

GO
--�����Ա�б�
CREATE TABLE [tbluGuildMember]
(
	[GNum] [int]  NOT NULL,															--������
	[UserID] [int] NOT NULL,														--��ұ�ʶID
	[Grade] [tinyint] NOT NULL,														--���ְλ 1:�����峤 2����ͨ��Ա
	[JoinTime] [datetime] NULL CONSTRAINT [DF_tbluGuildM_JoinTime] DEFAULT(getdate()),--�������ʱ��
	[UpdateTime] [datetime] NULL,													--��Ա��Ϣ����ʱ��
	[GMoney]	[int] NOT NULL CONSTRAINT [DF_tbluGuildM_GMoney] DEFAULT(0),		--��ԱĬ�ϵ�����
	CONSTRAINT [PK_tbluGuildMember] PRIMARY KEY CLUSTERED									--�����Ա����
	(
		[UserID]
	)WITH FILLFACTOR = 90 ON [PRIMARY]												--�����������,�ӿ��ȡ�ٶ�
) ON [PRIMARY]

GO
--�����������¼
CREATE TABLE [tbluGuildApplication]
(
	[UserID] [int] NOT NULL,														--��ұ�ʶID
	[GNum] [int]  NOT NULL,															--������
	[Grade] [tinyint] NOT NULL,														--���ְλ 1:�����峤 2����ͨ��Ա
	[ApplicationTime] [datetime] NULL CONSTRAINT [DF_tbluGuildA_AppTime] DEFAULT(getdate()),--�������ʱ��
	[BApproved] [int] NOT NULL CONSTRAINT [DF_tbluGuildA_BApproved] DEFAULT(0),	    --�Ƿ�����ͨ�� 0:δ���� 1���Ѿ�����												
	[ApprovedTime]	[datetime] NULL,												--����ʱ��
	CONSTRAINT [PK_tbluGuildApplication] PRIMARY KEY CLUSTERED									--���������������
	(
		[UserID],
		[GNum]
	)WITH FILLFACTOR = 90 ON [PRIMARY]												--�����������,�ӿ��ȡ�ٶ�
) ON [PRIMARY]

GO

-----------------------------------------------------------

--����������Ϣ
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO

CREATE PROCEDURE [GSP_GG_Guild_Create]
	@iUserID int,				--���Ψһ��ʶ
	@iGrade  tinyint,			--���ְλ ,�峤,��Ա
	@iGNum	 int OUTPUT,        --������
	@szGName  varchar(32),		--��������
	@iGMark	 tinyint,			--����ͼ����
	@iGType  tinyint,			--��������
	@iGLevel tinyint,			--����ȼ�
	@bRefuseJoin tinyint		--�Ƿ�ܾ���Ҽ���
AS
	SET NOCOUNT ON
	BEGIN TRAN
	--���������Ƿ��Ѿ�����
	IF EXISTS(SELECT GNum FROM tbluGuild WHERE GName = @szGName)
	BEGIN 
		ROLLBACK TRAN
		RETURN -1
	END
	--�Ѿ������ڼ������,�Ѿ��м���,���ܴ�������
	IF EXISTS(SELECT UserID FROM tbluGuildMember WHERE UserID = @iUserID)
	BEGIN 
		ROLLBACK TRAN
		RETURN -2
	END
	--�û�������
	IF EXISTS(SELECT UserID FROM tbluAccount WHERE UserID = @iUserID)
	BEGIN 
		ROLLBACK TRAN
		RETURN -3
	END
	--��������
    INSERT tbluGuild(GName,GMark,GType,GLevel,UpdateTime,bRefuseJoin)
	VALUES (@szGName,@iGMark,@iGType,@iGLevel,GETDATE(),@bRefuseJoin)

	IF 0 <> @@ERROR
	BEGIN
		ROLLBACK TRAN
		RETURN @@ERROR
	END

    SET @iGNum = @@IDENTITY
	--�������Ϣ���뵽�����Ա����
	INSERT tbluGuildMember (GNum,UserID,Grade,UpdateTime)
	VALUES (@iGNum,@iUserID,@iGrade,GETDATE())

	IF 0 <> @@ERROR
	BEGIN
		ROLLBACK TRAN
		RETURN @@ERROR
	END

	COMMIT TRAN
	RETURN 0

SET ANSI_NULLS ON
SET QUOTED_IDENTIFIER ON

GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO
--------------------------------------------------------
--ɾ������
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO

CREATE PROCEDURE [GSP_GG_Guild_Delete]
	@iGNum int					--�����ʶ
AS

	SET NOCOUNT ON
	BEGIN TRAN

	IF NOT EXISTS(SELECT GNum FROM tbluGuild WHERE GNum = @iGNum)
	BEGIN
		ROLLBACK TRAN
		RETURN -1
	END
	--ɾ��������Ϣ
	DELETE tbluGuild WHERE GNum = @iGNum
	IF 0 <> @@ERROR
	BEGIN
		ROLLBACK TRAN
		RETURN @@ERROR
	END
	--ɾ�������Ա��Ϣ
	DELETE tbluGuildMember WHERE GNum = @iGNum
	IF 0 <> @@ERROR
	BEGIN
		ROLLBACK TRAN
		RETURN @@ERROR
	END
	--ɾ�����������¼
	DELETE tbluGuildApplication WHERE GNum = @iGNum
	IF 0 <> @@ERROR
	BEGIN
		ROLLBACK TRAN
		RETURN @@ERROR
	END

	COMMIT TRAN
	RETURN 0
	
GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

-----------------------------------------------------------

--�޸ļ���������Ϣ
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO

CREATE PROCEDURE [GSP_GG_Guild_UpdateName]
	@iGNum int,					--�����ʶ
	@szGName varchar(32)        --��������
AS

	SET NOCOUNT ON
	IF NOT EXISTS(SELECT GNum FROM tbluGuild WHERE GNum = @iGNum)
	BEGIN
		RETURN -1
	END
	
	--�޸ļ�������
	UPDATE tbluGuild
	SET GName = @szGName
	WHERE GNum = @iGNum
	IF 0 <> @@ERROR
	BEGIN
		RETURN @@ERROR
	END

	RETURN 0
	
GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

-----------------------------------------------------------
--�޸ļ����Ƿ�������Ҽ���
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO

CREATE PROCEDURE [GSP_GG_Guild_UpdateRefuseJoin]
	@iGNum int,					--�����ʶ
	@bRefuseJoin tinyint
AS

	SET NOCOUNT ON
	IF NOT EXISTS(SELECT GNum FROM tbluGuild WHERE GNum = @iGNum)
	BEGIN
		RETURN -1
	END
	
	--�޸ļ�������
	UPDATE tbluGuild
	SET bRefuseJoin = @bRefuseJoin
	WHERE GNum = @iGNum
	IF 0 <> @@ERROR
	BEGIN
		RETURN @@ERROR
	END

	RETURN 0
	
GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO




